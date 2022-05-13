#include "client/client.h"
#include <sys/sendfile.h>
#include <stdio.h>

int parse_request(char* str,request* req) {
    printf("parsing req\n\n");
    printf("\n\n\n read request:%s", str);

    char *ptr = strtok(str, " ");
    while (ptr != NULL) //пока есть лексемы
    {
        printf("\n%s", ptr);
        ptr = strtok(NULL, " ,.");
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
    parse_request(read_bytes,req);
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
    if (request->method != "GET" && request->method != "HEAD") {
        response = "HTTP/1.1 405 Method Not Allowed\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
        return 0;
    }

    if (strstr(request->uri,"../") != NULL) {
        response = "HTTP/1.1 403 Forbidden\r\nServer: PreforkServer\r\nConnection: close\r\n\r\n";
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

#define MIMETYPE "mime-types"

char * get_mime_type(char *name) {
	char *ext = strrchr(name, '.');
  char delimiters[] = " ";
	char *mime_type = NULL;
	mime_type = malloc (128 * sizeof(char)) ;
	char line[128];
	char *token;
	int line_counter = 1;
	ext++; // skip the '.';
	FILE *mime_type_file = fopen(MIMETYPE, "r");
	if (mime_type_file != NULL) {
		while(fgets(line, sizeof line, mime_type_file) != NULL) {
			if (line_counter > 1)
			{
				if((token = strtok(line,delimiters)) != NULL) {
					if(strcmp(token,ext) == 0) {
						token = strtok(NULL, delimiters);
						strcpy(mime_type, token);
						break;
					}
				}
			}
			line_counter++;
		}
		fclose( mime_type_file );
	} else {
		perror("open");
	}
	return mime_type;
}
