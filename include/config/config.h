#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define DOCUMENT_ROOT_MAX_LENGTH 100

struct config {
    uint8_t cpu_limit;
    char document_root[DOCUMENT_ROOT_MAX_LENGTH];
};

typedef struct config config;

int read_config(char* path, config* conf);

#endif //CONFIG_H