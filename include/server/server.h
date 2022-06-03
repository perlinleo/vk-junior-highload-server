#ifndef STATIC_SERVER_SERVER_H
#define STATIC_SERVER_SERVER_H

#include <string>
#include <memory>

#include "config/config.h"
#include "client/client.h"

class Server {
public:
    Server(int port, Config* config);
    ~Server();
    std::shared_ptr<Client>connect();
private:
    int socket;
    std::string document_root;
};


#endif //STATIC_SERVER_SERVER_H
