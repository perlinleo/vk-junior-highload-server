#include "server/server.h"

#include <stdexcept>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>



Server::Server(int port, Config* config) {
    int sd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd <= 0) {
        throw std::runtime_error("socket: " + std::string(strerror(errno)));
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(sd);
        throw std::runtime_error("bind: " + std::string(strerror(errno)));
    }

    listen(sd, 40);
    socket = sd;
    document_root = config->document_root;
}

Server::~Server() {
    if (socket){
        close(socket);
    }
}

std::shared_ptr<Client> Server::connect() {
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    socklen_t client_size = sizeof(client);

    int client_sd = ::accept(socket, (struct sockaddr*)&client, &client_size);
    if (!client_sd) {
        return std::shared_ptr<Client>();
    }

    return std::make_shared<Client>(client_sd, document_root);
}