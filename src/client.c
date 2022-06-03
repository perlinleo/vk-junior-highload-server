#include "client/client.h"
#include <sys/sendfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int parse_request(char* str,request* req) {
    printf("parsing req\n\n");
    printf("\n\n\n read request:%s", str);

    char *ptr = strtok(str, " ");
    int counter = 0;
    while (ptr != NULL) //пока есть лексемы
    {
        printf("\n%s--", ptr);
        
        if (counter == 0) {
            printf("METHOD: %s\n", ptr);
            snprintf(req->method, 4, "%s", ptr);
        }
        if (counter == 1) {
            printf("URI: %s\n", ptr);
            snprintf(req->uri, 100, "%s", ptr);
        }

        ptr = strtok(NULL, " ");
        
        counter++;

    }
    fflush(stdout);
    return 0;
}

int create_client(client* c, int sock, char* document_root) {
    snprintf(c->document_root, 100, "%s", document_root);

    c->socket = sock;
}

int serve_client(client* c) {
    char* read_bytes = malloc(sizeof(char)*8196);
    int read = read_client(c,read_bytes);
    printf("Read bytes: %d\n", read);
    fflush(stdout);
    request* req = malloc(sizeof(request));
    int a = parse_request(read_bytes,req);
    if (a == 0) {
        char* path;
        int file_size = handler(req,c->response,c->document_root, path);
    }
    write_client(c,"/home/pierrelean/code/vk-junior-highload-server/build/cmake_install.cmake",100);
    free(read_bytes);

}

int read_client(client* c,char* output) {
    // ...
    size_t r = 0;  
    while (r < 8196) {
        size_t received = recv(c->socket,output+r, output-r,0);
        if(received == -1 || received == 0) {
            // free()?
            return 1;
        }

        r+= received;
        char* eor = strstr(output, "\r\n\r\n");
        if(eor != NULL) {
            printf("Found end of request");
            break;
        } else {
            printf("eor not found");
        }
        fflush(stdout);
    }
    return r;
}
int write_client(client* c,char* path, size_t fs) {
    send_client(c,c->response,strlen(c->response));

    if(c->request.method == "GET" && path!=NULL) {
        int fd = open(path,0);
        sendfile(c->socket,fd, 0, fs);
        close(fd);
    }
}
int send_client(client* c,const char* str, size_t ss) {
    size_t left = ss;
    ssize_t sent = 0;
    int flags = 0;

    while (left > 0) {
        sent = send(c->socket, str + sent, ss - sent, flags);
        if (!sent)
            printf("error");
        left -= sent;
    }
}

int url_decode(char* fn,char* output) {
    char* tmp;
    char* hexbuff = malloc(sizeof(char)*2);
    for (int i = 0; i < strlen(fn); ++i) {
        if (fn[i] == '%') { // decode character if in hex
            int sym;
            substring_from_to(fn,i+1,2,hexbuff);
            sscanf(hexbuff, "%x", &sym);
            char ch = (char)(sym);
            tmp += ch;
            i += 2;
        } else {
            tmp += fn[i];
        }
    }
}

void substring_from_to(char* string, size_t pos, size_t len, char* subbuff) {
    memcpy( subbuff, &string[pos], len );
    subbuff[len] = '\0';
}

int handler(request *request, char* response, char* doc_root, char* path) {
    printf("%s, %s---",request->method, request->uri);

    printf("%s\n\nCHECKING%s\n\n", request->method, request->uri);
    int checkMethod = strcmp(request->method, "GET");
    printf("GET? %d\n", checkMethod);
    int checkMethod1 = strcmp(request->method, "HEAD");
    printf("HEAD? %d\n", checkMethod1);
    
    if (checkMethod1 != 0 && checkMethod != 0) {
        response = "HTTP/1.1 405 Method Not Allowed\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
        printf("dsa");
        fflush(stdout);
        return 0;
    }

    char a = strstr(request->uri,"../");
    printf("\n--------    %c", a);
    
    if (!is_safe_path(request->uri)) {
        response = "HTTP/1.1 403 Forbidden\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
        printf("forbidden;\n");
        fflush(stdout);
        return 0;
    }

    char* e = strchr(request->uri,"?");
    if (e!= NULL) {
        int index = (int)(e-request->uri);  
        request->uri[index] = '\0';
    }

    char* buff = malloc(sizeof(char)*100);
    url_decode(request->uri,buff);
    
    // int slash = strrchr(request->uri,'/');

    // if ((slash = strrchr(request->uri,slash)) == request.uri.size() - 1) {
    //     if (slash > request.uri.rfind('.')) {
    //         response = "HTTP/1.0 404 Not Found\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
    //         return 0;
    //     }
    //     request.uri += "index.html";
    // }

    // pos = request.uri.rfind('.');
    // if (pos == std::string::npos) {
    //     response = "HTTP/1.0 400 Bad Request\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
    //     return 0;
    // }

    // bool bad_path = false;
    // path = document_root + request.uri;
    // int size = file_size(path);
    // catch (std::filesystem::filesystem_error& e) {
    //     bad_path = true;
    //     if (request.uri.rfind("index.html") != std::string::npos) {
    //         response = "HTTP/1.0 403 Forbidden\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
    //     } else {
    //         response = "HTTP/1.0 404 Not Found\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
    //     }
    // }
    // if (bad_path) {
    //     return 0;
    // }

    // char* mimeType = get_mime_type();
    // resp << "HTTP/1.1 200 OK\r\n"
    //      << "Content-Type: " << mimeType << "\r\n"
    //      << "Server: PreforkServer\r\n"
    //      << "Content-Length: " << size << "\r\n"
    //      << "Connection: close" << "\r\n\r\n";

    // response = std::move(resp.str());

    return 0;
}

static const char *const allowed_method[2] = {"GET", "HEAD"};
static const char *const connection_mod_str[] = {"close", "keep-alive"};

static const char *const status_code_name[] = {"200 OK", "400 Bad Request", "403 Forbidden",
                                                                        "404 Not Found", "405 Method Not Allowed",
                                                                        "500 Internal Server Error"};


static const char *const supported_mime_types[] = {"text/html", "text/css", "application/javascript",
                                                                          "image/jpeg", "image/png", "image/gif",
                                                                          "application/x-shockwave-flash"};


bool method_is_allowed(const char *method) {
    if (!method) {
        return false;
    }

    for (size_t i = 0; i < 2; i++) {
        if (strcmp(allowed_method[i], method) == 0)
            return true;
    }

    return false;
}

const char* unsafe_str = "..";

bool is_safe_path(const char *target_file_path) {
    if (!target_file_path) {
        return false;
    }

    const char* unsafe_sub_str = strstr(target_file_path, unsafe_str);
    if (strstr(target_file_path, unsafe_str)) {
        if (unsafe_sub_str == target_file_path || *(unsafe_sub_str - 1) == '/'){
            return false;
        }
    }

    return true;
}