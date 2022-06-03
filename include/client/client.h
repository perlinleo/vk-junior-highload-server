#ifndef STATIC_SERVER_CLIENT_H
#define STATIC_SERVER_CLIENT_H

struct Request {
    std::string method;
    std::string uri;
};

class Client {
public:
    Client(int sock, std::string doc);
    ~Client();
    void serve();
private:
    int socket;
    std::string document_root;
    Request request;
    std::string response;

    std::string read() const;
    void write(const std::string& path, int size_file);
    void send(const char* str, int size) const;
};

int parse(const std::string, Request&);

#endif //STATIC_SERVER_CLIENT_H
