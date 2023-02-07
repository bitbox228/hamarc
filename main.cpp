#include "parser.h"
#include "hamming.h"
#include "archiver.h"

void Archivation(const Options& options) {
    switch (options.operation) {
        case create:
            Create(options.arch_name, options.file_names, options.control_bits_count);
            break;
        case list:
            List(options.arch_name);
            break;
        case extract:
            Extract(options.arch_name, options.file_names);
            break;
        case append:
            Append(options.arch_name, options.file_names, options.control_bits_count);
            break;
        case del:
            Delete(options.arch_name, options.file_names);
            break;
        case concatenate:
            Concatenate(options.arch_name, options.file_names);
            break;
    }
}

int main(int argc, char** argv) {

    Options options;
    options.Parse(argc, argv);

    Archivation(options);

    return 0;
}