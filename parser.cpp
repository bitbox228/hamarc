#include "parser.h"

void Options::Parse(int argc, char** argv) {
    bool isArchName = false;
    bool isBits = false;
    for (size_t i = 1; i < argc; i++) {
        if (isArchName) {
            isArchName = false;
            arch_name = argv[i];
        } else if (isBits) {
            isBits = false;
            control_bits_count = atoi(argv[i]);
        } else if (argv[i][0] == '-') {
            if ((strcmp(argv[i], "--create") == 0) || (strcmp(argv[i], "-c") == 0)) {
                operation = create;
            } else if ((strcmp(argv[i], "--list") == 0) || (strcmp(argv[i], "-l") == 0)) {
                operation = list;
            } else if ((strcmp(argv[i], "--extract") == 0) || (strcmp(argv[i], "-x") == 0)) {
                operation = extract;
            } else if ((strcmp(argv[i], "--append") == 0) || (strcmp(argv[i], "-a") == 0)) {
                operation = append;
            } else if ((strcmp(argv[i], "--delete") == 0) || (strcmp(argv[i], "-d") == 0)) {
                operation = del;
            } else if ((strcmp(argv[i], "--concatenate") == 0) || (strcmp(argv[i], "-A") == 0)) {
                operation = concatenate;
            } else if (strcmp(argv[i], "-f") == 0) {
                isArchName = true;
            } else if (strcmp(argv[i], "-b") == 0) {
                isBits = true;
            } else {
                std::string arg = argv[i];
                std::string new_arg = arg.substr(0, 7);
                if (new_arg == "--file=") {
                    arch_name = arg.substr(7, arg.size() - 7);
                } else if (new_arg == "--bits=") {
                    control_bits_count = atoi(arg.substr(7, arg.size() - 7).c_str());
                }
            }
        } else {
            file_names.emplace_back(argv[i]);
        }
    }
}
