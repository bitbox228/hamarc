cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17)

project(HamArc)

add_executable(HamArc main.cpp parser.cpp parser.h archiver.cpp archiver.h hamming.cpp hamming.h file_encoder.cpp file_encoder.h)