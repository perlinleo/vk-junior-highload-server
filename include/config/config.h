#ifndef STATIC_SERVER_CONFIG_H
#define STATIC_SERVER_CONFIG_H

#include <string>

class Config {
public:
    int cpu_limit;
    std::string document_root;
    int readConfig(const std::string path);
};


#endif //STATIC_SERVER_CONFIG_H
