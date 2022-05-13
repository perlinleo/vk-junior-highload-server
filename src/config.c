#include "config/config.h"
#include <stdio.h>
#include <string.h>

#include <stdlib.h>

#define ERROR_CANT_OPEN_FILE 2
#define ERROR_FILE_READ 3
#define MAX_FILE_SIZE 1000

int read_config(char* path,config* conf){
    FILE *fp;
    char buf[MAX_FILE_SIZE];
    if ((fp=fopen(path, "rb"))==NULL) {
        return ERROR_CANT_OPEN_FILE;
    }
    size_t read = fread(buf,sizeof(char),MAX_FILE_SIZE,fp);

    if(!feof(fp)) return ERROR_FILE_READ;
    
    // printf(buf);

    char *ptr = strtok(buf, " ");

    while(ptr != NULL) 
    {
        if (strcmp(ptr, "cpu_limit") == 0) {
            ptr = strtok(NULL, " ");
            conf->cpu_limit = atoi(ptr);
            continue;
        }
        if (strcmp(ptr, "document_root") == 0) {
            ptr = strtok(NULL, " ");
            snprintf(conf->document_root, 100, "HELLO%s", ptr);

            //to fix
        }
        
        ptr = strtok(NULL, " ");
    }
    snprintf(conf->document_root, DOCUMENT_ROOT_MAX_LENGTH, "static", ptr);

    fclose(fp);
    return 0;
};