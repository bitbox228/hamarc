#include <vector>
#include <string>

enum operations {create, list, extract, append, del, concatenate};

struct Options {
    operations operation;

    std::string arch_name;
    std::vector<std::string> file_names;
    unsigned char control_bits_count;

    void Parse(int argc, char** argv);
};