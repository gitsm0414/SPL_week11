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
  if((listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
	  printf("socket() failed.\n");
	  exit(2);
  }

  /* Bind sockaddr (IP, etc.) to listen socket */
  memset((char *)&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_port = htons(port);
  if(bind(listen_fd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0){
	  printf("bind() failed.\n");
	  exit(3);
  }

  /* Listen to listen socket */
  if(listen(listen_fd, 5) < 0){
	  printf("listen() failed.\n");
	  exit(4);
  }

  while (1) {
    /* Accept connection request from clients */
    caddrlen = sizeof(caddr);
    if((conn_fd = accept(listen_fd, (struct sockaddr *)&caddr, (socklen_t *)&caddrlen)) < 0){
	    printf("accept() failed.\n");
	    continue;
    }

    int nbytes;
    while (1) {
      /* Get filename from client */
      if((nbytes = read(conn_fd, buffer, sizeof(buffer))) <= 0) {
		      continue;
      }
      buffer[nbytes] = '\0';
      
      if(strncmp(buffer, "-quit", 5) == 0){
      	break;
      }

      /* Print filename */
      printf("File Name: %s\n", buffer);

      /* Create a new file called <filename>_copy  */
      int fd;
      char* path = (char*)malloc(sizeof(char) * 55);
      sprintf(path, "./%s_copy", buffer);
      
      if((fd = open(path, O_WRONLY|O_CREAT, 0777)) < 0){
      	printf("file opening error\n");
	free(path);
	exit(5);
      }
      free(path);
      /* Send ack */
      buffer_data[0] = '-';
      buffer_data[1] = 'a';
      buffer_data[2] = 'c';
      buffer_data[3] = 'k';
      buffer_data[4] = '-';

      write(conn_fd, buffer_data, 5);
      
      int cnt =0;
      /* Receive data and save data to <filename> _copy */
      while(1){
      nbytes = read(conn_fd, buffer_data, MAXLINE);
      if(nbytes == -1){
	      printf("file reading error\n");
	      exit(5);
      }
      else if(nbytes >= 0){
      	if(strncmp(buffer_data, "-EOF", 4) == 0){
		close(fd);
		break;
	}
	else{
		write(fd, buffer_data, nbytes);
		cnt+=nbytes;
	}
      }
     }

      /* Print numbers of bytes received. */
      printf("Received %d bytes.\n", cnt);

      /* Break from loop once client quits */
    }

    /* Receive filename & data */
   }

  close(conn_fd);
  
  return 0;
}
