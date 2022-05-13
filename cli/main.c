
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <config/config.h>
#include <server/server.h>
#include <client/client.h>

int main() {
  config *conf =(malloc(sizeof(struct config)));
  read_config("httpd.conf",conf);
  printf("CPU limit: %d\nDocument root: %s", conf->cpu_limit, conf->document_root);
  config *server =(malloc(sizeof(struct server)));
  printf("hello");
  int a = create_server(8080, &conf,server);
  fflush(stdout);

  for(int i = 0; i <= conf->cpu_limit; i++) {
    printf("%d",i);
    pid_t pid;
    if((pid = fork()) > 0) {
      break;
    } else if ( pid < 0 ) {
      printf("Fork failed for pid %d", pid);
    }
  }
  for(;;) {
      // printf("hello");
      client *client =(malloc(sizeof(client)));
      connect_server(server, client);
      serve_client(client);
      free(client);
  }
  return 0;
}