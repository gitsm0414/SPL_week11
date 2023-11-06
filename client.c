#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <fcntl.h>
#define MAXLINE 80

int main(int argc, char *argv[]) {
  struct hostent *host_entry;
  struct sockaddr_in saddr;
  char buffer[MAXLINE], buffer_data[MAXLINE];
  int conn_fd;

  if (argc < 3) {
    printf(
        "Received %d arguments. Please enter host address and port number!\n",
        argc - 1);
    exit(1);
  }

  char *host = argv[1];
  in_port_t port = (in_port_t)strtol(argv[2], NULL, 10);
  if (errno == ERANGE) {
    printf("invalid port number %s\n", argv[2]);
    exit(1);
  }

  /* Create socket */
  if((conn_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
	  printf("socket() failed.\n");
	  exit(2);
  }
  /* Get host information */
  if((host_entry = gethostbyname(host)) == NULL){
	  printf("invalid host name %s\n", host);
	  exit(3);
  }
  memset((char*)&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  memcpy((char*)&saddr.sin_addr.s_addr, (char*)host_entry->h_addr, host_entry->h_length);
  saddr.sin_port = htons(port);

  /* Connect to host */
  if(connect(conn_fd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0){
	  printf("connect() failed.\n");
	  exit(4);
  }
  
  int nbytes;
  
  while (1) {
    /* Get filename from user */
    if((nbytes = read(0, buffer, MAXLINE)) <= 0) continue;
    if(strncmp(buffer, "quit", 4) == 0){
    	buffer_data[0] = '-';
	buffer_data[1] = 'q';
	buffer_data[2] = 'u';
	buffer_data[3] = 'i';
	buffer_data[4] = 't';

	write(conn_fd, buffer_data, 5);
	break;
    }

    /* Send filename to host */
    buffer[nbytes-1]='\0';
    write(conn_fd, buffer, nbytes-1);
    printf("Sending %s...\n", buffer);

    /* Receive ack from host */
    while(1){
    	read(conn_fd, buffer_data, MAXLINE);
        if(strncmp(buffer_data, "-ack-", 5) == 0) break;
    }
    /* Read the file and send data to host */
    int fd;
    char* path;
    sprintf(path, "./%s", buffer);

    if((fd = open(path, O_RDONLY)) < 0 ){
	    printf("file opening error\n");
	    exit(5);
    }

    int cnt = 0;
    while((nbytes = read(fd, buffer_data, MAXLINE)) > 0){
	    write(conn_fd, buffer_data, nbyets);
	    cnt += nbytes;
    }

    /* Send EOF */
    buffer_data[0] = '-';
    buffer_data[1] = 'E';
    buffer_data[2] = 'O';
    buffer_data[3] = 'F';
    close(fd);

    write(conn_fd, buffer_data, 4);

    /* Print number of bytes sent */
    printf("Sent %d bytes.\n", cnt);  
      
  }

  close(conn_fd);

  return 0;
}
