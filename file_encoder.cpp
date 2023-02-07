#include "file_encoder.h"

const uint16_t kCountHeaderSize = 8;
const uint16_t kEncodedCountHeaderSize = kCountHeaderSize * 2;
const uint16_t kNameHeaderSize = 25;
const uint16_t kEncodedNameHeaderSize = kNameHeaderSize * 2;
const uint16_t kBits[] = {0, 4};
const std::string arch_tmp_name = "arch.tmp";

uint64_t Pow(uint64_t left, unsigned char right) {
    uint64_t result = 1;
    for (size_t i = 0; i < right; i++) {
        result *= left;
    }
    return result;
}

void AddEncodedByteToArchive(unsigned char current_byte, std::ofstream& arch) {
    bool current_bits[CHAR_BIT];
    size_t control_bit;
    std::vector<bool> hamming_code(CHAR_BIT, false);

    for (size_t j = 0; j < CHAR_BIT; j++) {
        current_bits[CHAR_BIT - j - 1] = (current_byte >> j) & 1;
    }

    for (uint16_t bits_count: kBits) {
        control_bit = 1;
        for (size_t i = 0; i < CHAR_BIT; i++) {
            if (control_bit - 1 == i) {
                control_bit *= 2;
            } else {
                hamming_code[i] = current_bits[bits_count];
                bits_count++;
            }
        }
        EncodeHamming(hamming_code);
        current_byte = 0;
        for (int i = 0; i < CHAR_BIT; i++) {
            if (hamming_code[i]) {
                current_byte += static_cast<unsigned char>(pow(2, CHAR_BIT - i - 1));
            }
        }
        arch << current_byte;
    }
}

void archiver::AddEncodedHammingToFile(const file& input, std::ofstream& output) {
    unsigned char current_byte;

    std::ifstream file_stream;
    file_stream.open(input.name, std::ios::binary);

    if (input.control_bits == 3) {
        for (size_t i = 0; i < input.bytes_count; i++) {
            current_byte = file_stream.get();
            AddEncodedByteToArchive(current_byte, output);
        }
    } else if (input.control_bits >= 4) {
        uint64_t hamming_bits_count = Pow(2, input.control_bits);
        uint64_t information_bits_count = hamming_bits_count - input.control_bits - 1;
        std::vector<bool> current_hamming_bits(hamming_bits_count, false);
        std::vector<bool> next_hamming_bits(CHAR_BIT, false);
        uint64_t prev_bits_count = 0;
        uint64_t current_bytes_count = 0;
        uint64_t bytes_add = 0;
        uint64_t control_bit = 1;
        uint64_t current_bit = 0;
        uint64_t next_bits_count = 0;
        bool current_bits[CHAR_BIT];

        while ((current_bytes_count < input.bytes_count) || (prev_bits_count != 0)) {
            bytes_add =
                    (information_bits_count - prev_bits_count) / 8 +
                    ((information_bits_count - prev_bits_count) % 8 != 0);
            for (size_t i = 0; (i < bytes_add) && (current_bytes_count < input.bytes_count); i++) {
                current_bytes_count++;
                current_byte = file_stream.get();

                for (size_t j = 0; j < CHAR_BIT; j++) {
                    current_bits[CHAR_BIT - j - 1] = (current_byte >> j) & 1;
                }

                for (size_t j = 0; j < CHAR_BIT; j++) {
                    while (current_bit == control_bit - 1) {
                        current_bit++;
                        control_bit *= 2;
                    }
                    if (current_bit >= hamming_bits_count) {
                        next_hamming_bits[next_bits_count++] = current_bits[j];
                    } else {
                        current_hamming_bits[current_bit++] = current_bits[j];
                    }
                }
            }

            EncodeHamming(current_hamming_bits);

            for (uint64_t i = 0; i < hamming_bits_count; i += 8) {
                current_byte = 0;
                for (size_t j = 0; j < CHAR_BIT; j++) {
                    if (current_hamming_bits[i + j]) {
                        current_byte += static_cast<unsigned char>(Pow(2, CHAR_BIT - j - 1));
                    }
                }
                output << current_byte;

            }

            for (uint64_t i = 0; i < hamming_bits_count; i++) {
                current_hamming_bits[i] = false;
            }

            current_bit = 0;
            control_bit = 1;
            for (int i = 0; i < next_bits_count; i++) {
                while (current_bit == control_bit - 1) {
                    current_bit++;
                    control_bit *= 2;
                }
                current_hamming_bits[current_bit++] = next_hamming_bits[i];
            }
            prev_bits_count = next_bits_count;
            next_bits_count = 0;
        }
    }
}