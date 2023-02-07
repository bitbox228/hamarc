#include "hamming.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <cstdio>

void Create(const std::string& arch_name, const std::vector<std::string>& file_names, unsigned char control_bits_count);

void List(const std::string& arch_name);

void Extract(const std::string& arch_name, const std::vector<std::string>& file_names);

void Append(const std::string& arch_name, const std::vector<std::string>& file_names, unsigned char control_bits_count);

void Delete(const std::string& arch_name, const std::vector<std::string>& file_names);

void Concatenate(const std::string& arch_name, const std::vector<std::string>& file_names);