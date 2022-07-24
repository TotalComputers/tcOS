#include "file.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::string readFile(const std::string& path) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(path);
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        return ss.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "Unable to read file `" << path << "`: " << e.what() << std::endl;
    }
    return "";
}