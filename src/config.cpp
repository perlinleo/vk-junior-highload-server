#include "config/config.h"

#include <iostream>
#include <sstream>
#include <fstream>

int Config::readConfig(const std::string path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return 0;
    }

    std::string line;
    while (getline(file, line)) {
        std::istringstream stream(line);
        std::string field, value;
        stream >> field >> value;

        if (field == "cpu_limit") {
            this->cpu_limit = std::atol(value.c_str());
        } else if (field == "document_root") {
            this->document_root = value;
        }
    }

    return 1;
}
