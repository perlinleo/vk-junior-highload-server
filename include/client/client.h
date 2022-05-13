#ifndef CLIENT_H
#define CLIENT_H

#include <stddef.h>
#include <sys/socket.h>


struct request {
    char method[100];
    char uri[100];
};

typedef struct request request;

int parse_request(char* str,request* req);

struct client {
    int socket;
    request request;
    char document_root[100];
    char response[2048];
};

typedef struct client client;

int create_client(client* c, int sock, char* document_root);
int read_client(client* c,char* output);
int write_client(client* c,char* path, size_t fs);
int send_client(client* c,const char* str, size_t ss);
int serve_client(client* c);

#endif //CLIENT_H