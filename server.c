#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define MAXLINE 80

int main(int argc, char *argv[]) {
  int listen_fd, conn_fd, caddrlen;
  struct sockaddr_in saddr, caddr;
  char buffer[MAXLINE], buffer_data[MAXLINE];

  if (argc < 2) {
    printf("Received %d arguments. Please enter port number!\n", argc - 1);
    exit(1);
  }
  in_port_t port = (in_port_t)strtol(argv[1], NULL, 10);
  if (errno == ERANGE) {
    printf("invalid port number %s\n", argv[1]);
    exit(1);
  }

  /* Create listen socket */

  /* Bind sockaddr (IP, etc.) to listen socket */

  /* Listen to listen socket */

  while (1) {
    /* Accept connection request from clients */

    while (1) {
      /* Get filename from client */

      /* Print filename */

      /* Create a new file called <filename>_copy  */

      /* Send ack */

      /* Receive data and save data to <filename> _copy */

      /* Print numbers of bytes received. */

      /* Break from loop once client quits */
    }

    /* Receive filename & data */
  }

  close(conn_fd);

  return 0;
}
