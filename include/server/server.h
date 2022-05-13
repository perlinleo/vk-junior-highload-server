#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include "config/config.h"
#include "client/client.h"

struct server {
    uint16_t socket;
    config conf;
};

typedef struct server server;

int create_server(uint16_t port, config* conf,server* serv);
int connect_server(server* serv, client* c);

#endif //SERVER_H