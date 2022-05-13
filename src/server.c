#include <server/server.h>
#include <client/client.h>
#include <sys/socket.h>
#include <netinet/in.h>

int create_server(uint16_t port, config* conf,server* serv) {
    int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd <= 0) {
        printf("error");
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printf("error 2");
        close(sd);
        return 2;
    }
    listen(sd, 40);
    printf("%d", sd);
    serv->socket = sd;
    serv->conf = *conf;
    return 0;
}   

int connect_server(server* serv, client* c) {
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    socklen_t client_size = sizeof(client);
    int client_sd = accept(serv->socket, (struct sockaddr*)&client, &client_size);
    if (client_sd<0) {
        return 1;
    }
    printf("%d", client_sd);
    create_client(c,client_sd, serv->conf.document_root);
    return 0;
}