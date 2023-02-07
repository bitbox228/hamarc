#include "hamming.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <cstdio>


namespace archiver{

    uint64_t BytesCount(std::ifstream& file) {
        char current;
        size_t bytes = 0;
        while (file.get(current)) {
            bytes++;
        }
        file.clear();
        file.seekg(0, std::ios::beg);
        return bytes;
    }

    struct file {

        std::string name;

        uint64_t bytes_count = 0;

        unsigned char control_bits = 3; /// hamming bits count

        file() = default;

        file(const std::string& file_name): name(file_name) {
            std::ifstream input;
            input.open(file_name);
            bytes_count = BytesCount(input);
        }

        file(const std::string& file_name, unsigned char ham_bits): file(file_name) {
            this->control_bits = ham_bits;
        }

    };

    void AddEncodedHammingToFile(const file& input, std::ofstream& output);

    struct Node { /// Hamming Node

        bool is_leaf;

        Node* right = nullptr; /// one
        Node* left = nullptr; /// zero

        uint64_t freq = 0;

        unsigned char value;

        Node() = default;

        Node(unsigned char value): value(value){
            is_leaf = true;
        }

        Node(unsigned char value, uint64_t freq): value(value), freq(freq){
            is_leaf = true;
        }

        Node(Node* left, Node* right): left(left), right(right){
            is_leaf = false;
            this->freq = left->freq + right->freq;
        }
    };

    bool operator<(const Node& lhs, const Node& rhs) {
        return lhs.freq < rhs.freq;
    }

    bool operator>(const Node& lhs, const Node& rhs) {
        return lhs.freq > rhs.freq;
    }

    bool operator<=(const Node& lhs, const Node& rhs) {
        return lhs.freq <= rhs.freq;
    }

    bool operator>=(const Node& lhs, const Node& rhs) {
        return lhs.freq >= rhs.freq;
    }

    bool operator==(const Node& lhs, const Node& rhs) {
        return lhs.freq == rhs.freq;
    }

    struct HuffmanTree {

        Node* root = nullptr;


    };

    std::vector<Node*> CountFreq(const file& input) {
        unsigned char current_byte;
        std::unordered_map<unsigned char, uint64_t> freq;
        std::ifstream file_stream;
        file_stream.open(input.name, std::ios::binary);

        for (size_t i = 0; i < input.bytes_count; i++) {
            current_byte = file_stream.get();
            freq[current_byte]++;
        }



    }

}
