#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>

int main(int argc, char *argv[])
{
char buffer[1024],b[2],cpy[1024],cpy2[1024];
int server_sockfd, client_sockfd,port,check;
int optval = 1;
socklen_t server_len, client_len;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
FILE *fp;

    if( argc == 2 ) {
        port = atoi(argv[1]);
     }
     else if( argc > 2 ) {
        printf("Too many arguments supplied.\n");
        exit(0);
     }
     else {
        printf("One argument expected.\n");
        exit(0);
    }

server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
//not usefull
if (setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval)) == -1) {
    printf("setsockopt");
    exit(1);
}
server_address.sin_family = AF_INET;
server_address.sin_addr.s_addr = htonl(INADDR_ANY);
server_address.sin_port = htons(port);
server_len = sizeof(server_address);
bind(server_sockfd, (struct sockaddr *)&server_address,server_len);

/* Create a connection queue, ignore child exit details and wait for
clients. */

listen(server_sockfd, 5);

//signal(SIGCHLD, SIG_IGN);

while(1) {


printf("server waiting\n");

/* Accept connection. */

client_len = sizeof(client_address);
client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, &client_len);

/* Fork to create a process for this client and perform a test to see
whether we're the parent or the child. */

if(fork() == 0) {

/* If we're the child, we can now read/write to the client on
client_sockfd.
The five second delay is just for this demonstration. */
do{
//bzero(b, 2);

if(read(client_sockfd, b, 2))
{

//sleep(5);
//ch++;
if(!strcmp(b,"1")){
  //printf("gone1");
  fp = fopen("data.txt", "a+");
  bzero(buffer, 1024);
  read(client_sockfd, buffer, 1024);
  strcat(buffer,"\n");
  check = 0;
  while(fgets(cpy2, 1024, fp) != NULL) {
		if((strstr(cpy2, buffer)) != NULL) {
			write(client_sockfd, "1", 1);
			check++;
      break;
		}
  }
  if(check < 1)
  {
    write(client_sockfd, "2", 1);
  }

}
else if(!strcmp(b,"2")){
  //printf("gone2");
  //write(client_sockfd, "2", 1);
  fp = fopen("data.txt", "a+");
  bzero(buffer, 1024);
  read(client_sockfd, buffer, 1024);
  strcpy(cpy,buffer);
  strcat(cpy," ");
  check = 0;
  while(fgets(cpy2, 1024, fp) != NULL) {
		if((strstr(cpy2, cpy)) != NULL) {
			write(client_sockfd, "1", 1);
			check++;
      break;
		}
		//line_num++;
	}

  if(check < 1)
  {
    //unsername not found
      write(client_sockfd, "2", 1);
      bzero(cpy2, 1024);
      read(client_sockfd, cpy2, 1024);
      fprintf(fp, "%s %s\n",buffer,cpy2);
  }


  //fprintf(fp, "%s\n",cpy);

  fclose(fp);



}
}
//printf("\n");
//write(client_sockfd, buffer, 1023);



}
while(1);
close(client_sockfd);
exit(0);
}

/* Otherwise, we must be the parent and our work for this client is
finished. */

else {
close(client_sockfd);
}
}
}
