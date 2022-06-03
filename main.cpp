#include <iostream>
#include <unistd.h>

#include "config/config.h"
#include "server/server.h"
#include "client/client.h"


#define SERVER_PORT 80

int main() {
    Config config;
    int isOk = config.readConfig("/etc/httpd.conf");
    if (!isOk) {
        std::cout << "config parsing fail" << std::endl;
        return -1;
    }

    std::cout << "cpu_limit: " << config.cpu_limit << std::endl;
    std::cout << "document_root: " << config.document_root << std::endl;

    try {
        Server server(SERVER_PORT, &config);
        for (int i = 0; i < config.cpu_limit - 1; ++i) {
            pid_t pid;
            if ((pid = fork())) {
                break;
            } else if (pid < 0) {
                std::cout << "fork fail" << std::endl;
            }
        }

        for(;;) {
            std::shared_ptr<Client> client = server.connect();
            client->serve();
        }
    }
    catch (std:: exception& ex) {
        std::cout << "exception:   " << ex.what() << std::endl;
    }

    return 0;
}
