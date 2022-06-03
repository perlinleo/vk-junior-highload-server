#include <iostream>
#include <unistd.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <cstring>
#include "client/client.h"
#include <string>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <filesystem>


Client::Client(int sock, std::string doc): socket(sock), document_root(doc){}

Client::~Client() {
    if (socket) {
        close(socket);
    }
}

void errorHandler(std::string& resp) {
    resp = "HTTP/1.0 400 Bad Request\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
}

std::unordered_map<std::string, std::string> mime_types = { {".txt", "text/plain"},
                                                            {".html", "text/html"},
                                                            {".css", "text/css"},
                                                            {".js", " text/javascript"},
                                                            {".png", "image/png"},
                                                            {".jpg", "image/jpeg"},
                                                            {".jpeg", "image/jpeg"},
                                                            {".png", "image/png"},
                                                            {".swf", "application/x-shockwave-flash"},
                                                            {".gif", "image/gif"},
                                                            {".mp4", "video/mp4"}};


std::string get_mime_type(std::string extension) {
    std::transform(extension.begin(), extension.end(), extension.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    auto it = mime_types.find(extension);
    if (it == mime_types.end()) {
        return "application/octet-stream";
    }

    return it->second;
}


std::string url_decode(std::string& fileName) {
    std::string tmp;
    for (int i = 0; i < fileName.size(); ++i) {
        if (fileName[i] == '%') { // decode character if in hex
            int sym;
            sscanf(fileName.substr(i + 1, 2).c_str(), "%x", &sym);
            char ch = static_cast<char>(sym);
            tmp += ch;
            i += 2;
        } else {
            tmp += fileName[i];
        }
    }

    return tmp;
}

int handler(Request request, std::string& response, std::string document_root, std::string& path) {
    if (request.method != "GET" && request.method != "HEAD") {
        response = "HTTP/1.1 405 Method Not Allowed\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
        return 0;
    }

    if (request.uri.find("../") != std::string::npos) {
        response = "HTTP/1.1 403 Forbidden\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
        return 0;
    }

    auto pos = request.uri.find('?');
    if (pos != std::string::npos) {
        request.uri = request.uri.substr(0, pos);
    }

    request.uri = url_decode(request.uri);

    int slash;
    if ((slash = request.uri.rfind('/')) == request.uri.size() - 1) {
        if (slash > request.uri.rfind('.')) {
            response = "HTTP/1.0 404 Not Found\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
            return 0;
        }
        request.uri += "index.html";
    }

    pos = request.uri.rfind('.');
    if (pos == std::string::npos) {
        response = "HTTP/1.0 400 Bad Request\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
        return 0;
    }

    bool bad_path = false;
    path = document_root + request.uri;
    std::cout << path;
    int size = 0;
    try {
        size = std::filesystem::file_size(path);
    }
    catch (std::filesystem::filesystem_error& e) {
        bad_path = true;
        if (request.uri.rfind("index.html") != std::string::npos) {
            response = "HTTP/1.0 403 Forbidden\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
        } else {
            response = "HTTP/1.0 404 Not Found\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
        }
    }
    if (bad_path) {
        return 0;
    }

    std::string mimeType = get_mime_type(request.uri.substr(pos));
    std::stringstream resp;
    resp << "HTTP/1.1 200 OK\r\n"
         << "Content-Type: " << mimeType << "\r\n"
         << "Server: PreforkServer\r\n"
         << "Content-Length: " << size << "\r\n"
         << "Connection: close" << "\r\n\r\n";

    response = std::move(resp.str());

    return size;
}


void Client::serve() {
    std::string requestStr = read();
    try {
        int isParsed = parse(requestStr, request);
        if (isParsed) {
            std::string path;
            int file_size = handler(request, response, document_root, path);
            write(path, file_size);
        } else {
            errorHandler(response);
            write("", 0);
        }
    }
    catch (std:: exception& ex) {
        std::cout << "exception:   " << ex.what() << std::endl;
    }
}

std::string Client::read() const {
    std::string result;
    int bytes = 8000;
    char *buf = new char[bytes];
    size_t r = 0;
    while (r != bytes) {
        ssize_t received = ::recv(socket, buf + r, bytes - r, 0);
        if (received == -1 || received == 0) {
            delete [] buf;
            return result;
        }

        r += received;
        result.append(buf, received);
        if (result.find("\r\n\r\n") != std::string::npos) {
            break;
        }
    }

    delete [] buf;
    return result;
}

void Client::write(const std::string &path, int size_file) {
    send(response.data(), response.size());

    if (request.method == "GET" && !path.empty()) {
        int fd = open(path.c_str(), 0);
        ::sendfile(socket, fd, 0, size_file);
        close(fd);
    }
}

void Client::send(const char *str, int size) const {
    size_t left = size;
    ssize_t sent = 0;
    int flags = 0;

    while (left > 0) {
        sent = ::send(socket, str + sent, size - sent, flags);
        if (!sent)
            throw std::runtime_error("write fail: " + std::string(strerror(errno)));
        left -= sent;
    }
}

int parse(const std::string from, Request& to) {
    std::stringstream ss(from);
    ss >> to.method;
    ss >> to.uri;

    std::string input;
    while (ss >> input) {
        if (to.method == "GET" ||
            to.method == "HEAD" ||
            to.method == "OPTIONS" ||
            to.method == "PUT" ||
            to.method == "DELETE" ||
            to.method == "POST") {
            return 1;
        } else {
            return -1;
        }
    }

    return -1;
}








