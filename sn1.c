/*
    Simple udp server
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<pthread.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<time.h>

#define SERVER "127.0.0.1"
#define BUFLEN 1024  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

int port1;
int port2;
int port3;

//char buf1[1024];

char * requestSN(char *);
char * requestSN2(char *);
char * requestSN3(char *);


void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in si_me, si_other, si_other1;

    int s, i, s1, s2, slen = sizeof(si_other), slen1 = sizeof(si_other1) , recv_len, check;
    char buf[BUFLEN], buf1[BUFLEN], cpy[1024],cpy2[BUFLEN],cpy3[BUFLEN];
    int port;
    pthread_t threads;
    FILE *fp;

    if( argc == 5 ) {
        port = atoi(argv[1]);
        port1 = atoi(argv[2]);
        port2 = atoi(argv[3]);
        port3 = atoi(argv[4]);
     }
     else if( argc > 5 ) {
        printf("Too many arguments supplied.\n");
        exit(0);
     }
     else {
        printf("\n4 argument expected.\n");
        exit(0);
    }


    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(port);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }

    //keep listening for data
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);
        bzero(buf,BUFLEN);
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }
        printf("\noption:%s\n",buf);
        if(strlen(buf)==1 && strcmp(buf,"1")==0)
        {//client request for friend list
          printf("\nclient\n");
          bzero(buf,BUFLEN);
          /*
          if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
          {
              printf("\nfriend name:%s\n",buf);
              fp = fopen("SuperNode1.txt", "a+");
              check = 0;
              bzero(cpy,1024);
              while(fgets(cpy, 1024, fp) != NULL) {
                if((strstr(cpy, buf)) != NULL) {
                  sendto(s, cpy, strlen(cpy), 0, (struct sockaddr*) &si_other, slen);
                  check++;
                  break;
                }

              }
              if(check == 0)
              {

                strcpy(buf,requestSN(buf));

                //strcpy(buf,requestSN(buf));
                sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &si_other, slen);
                bzero(buf,BUFLEN);


              }
          }
          */
          bzero(buf1,BUFLEN);
          if (recvfrom(s, buf1, BUFLEN, 0, (struct sockaddr *) &si_other, &slen))
          {
            fp = fopen("SuperNode1.txt", "a+");
            printf("data:- %s\n",buf1);

            check=0;

            while(fgets(cpy, 1024, fp) != NULL) {
              if((strstr(cpy, buf1)) != NULL) {
                sendto(s, cpy, strlen(cpy), 0, (struct sockaddr*) &si_other, slen);
                check++;
                break;
              }

            }
            if(check == 0)
            {
              bzero(buf,BUFLEN);
              printf("\nsending request for '%s' to other supernodes\n",buf1);
              bzero(cpy2,BUFLEN);
              strcpy(cpy2,buf1);
              bzero(cpy3,BUFLEN);
              strcpy(cpy3,buf1);
              strcpy(buf,requestSN(buf1));
              if(strcmp(buf,"No")==0){
                printf("\nrequest data:%s\n",cpy2);
                strcpy(buf,requestSN2(cpy2));
              }
              if(strcmp(buf,"No")==0){
                printf("\nrequest data:%s\n",cpy3);
                strcpy(buf,requestSN3(cpy3));
              }
              //strcpy(buf,requestSN(buf));
              sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &si_other, slen);
              //sendto(s, "No", strlen("No"), 0, (struct sockaddr*) &si_other, slen);

              bzero(buf,BUFLEN);


            }





            fclose(fp);
          }
        }
        else if(strlen(buf)==1 && strcmp(buf,"2")==0)
        { //fRequest from other supernodes
          printf("\nSuperNode\n");
          bzero(buf1,BUFLEN);


          if (recvfrom(s, buf1, BUFLEN, 0, (struct sockaddr *) &si_other, &slen))
          {
              printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
              bzero(cpy,1024);
              fp = fopen("SuperNode1.txt", "a+");
              check = 0;
              while(fgets(cpy, 1024, fp) != NULL) {
                if((strstr(cpy, buf1)) != NULL) {
                  if(sendto(s, cpy, strlen(cpy), 0, (struct sockaddr*) &si_other, slen) < 0)
                  {
                    die("recvfrom()");
                  }
                  check++;
                  break;
                }
              }
              if(check == 0)
              {
                if(sendto(s, "No", strlen("No"), 0, (struct sockaddr*) &si_other, slen) < 0)
                {
                  die("recvfrom()");
                }
              }
              fclose(fp);
          }



        }
        else if(strlen(buf)==1 && strcmp(buf,"3")==0)
        {//new user requesting to register himself at supernode
          bzero(buf1,BUFLEN);
          if (recvfrom(s, buf1, BUFLEN, 0, (struct sockaddr *) &si_other, &slen))
          {
            fp = fopen("SuperNode1.txt", "a+");
            printf("data:- %s\n",buf1);
            if(fprintf(fp, "%s\n",buf1))
            {
              if(sendto(s, "YES", strlen("YESS"), 0, (struct sockaddr*) &si_other, slen) < 0)
              {
                die("recvfrom()");
              }
            }
            else{
              if(sendto(s, "No", strlen("No"), 0, (struct sockaddr*) &si_other, slen) < 0)
              {
                die("recvfrom()");
              }
            }
            fclose(fp);
          }
        }


    }

    close(s);
    return 0;
}


char * requestSN(char * buf){

  struct sockaddr_in si_other, si_me;
  int s, i, slen=sizeof(si_other),k;
  char buf1[1024];
  char message[BUFLEN];
  srand(time(NULL));
  //printf("da: %s",buf1);
  k = (rand() % 40000) + 2000 ;
  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
      die("socket");
  }


  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(k);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  //bind socket to port

  if ( bind(s, (struct sockaddr *)&si_me, sizeof(si_me)) != 0 )
	{
		printf("Bind");
		exit(1);
	}
  printf("\nData about to send:%s\n",buf);




  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(port1);

  if (inet_aton(SERVER , &si_other.sin_addr) == 0)
  {
      printf("inet_aton failed\n");
      exit(1);
  }


  if ( connect(s, (struct sockaddr *)&si_other, sizeof(si_other)) != 0 )
  {
    printf("Connect");
    exit(1);
  }


      if (send(s, "2", strlen("1") , 0 )==-1)
      {
          die("sendto()");
      }


      //send the message
      printf("data--:%s\n",buf);
      if (send(s, buf, strlen(buf) , 0 )==-1)
      {
          die("sendto()");
      }

      //receive a reply and print it
      //clear the buffer by filling null, it might have previously received data
      bzero(buf1, BUFLEN);
      //try to receive some data, this is a blocking call
      if (recv(s, buf1, BUFLEN, 0) == -1)
      {
          die("recvfrom()");
      }

      strcpy(buf,buf1);

  close(s);

  return buf;

}


char * requestSN2(char * buf){

  struct sockaddr_in si_other, si_me;
  int s, i, slen=sizeof(si_other),k;
  char buf1[1024];
  char message[BUFLEN];
  srand(time(NULL));
  //printf("da: %s",buf1);
  k = (rand() % 40000) + 2000 ;
  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
      die("socket");
  }


  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(k);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  //bind socket to port

  if ( bind(s, (struct sockaddr *)&si_me, sizeof(si_me)) != 0 )
	{
		printf("Bind");
		exit(1);
	}
  printf("\nData about to send:%s\n",buf);




  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(port2);

  if (inet_aton(SERVER , &si_other.sin_addr) == 0)
  {
      printf("inet_aton failed\n");
      exit(1);
  }


  if ( connect(s, (struct sockaddr *)&si_other, sizeof(si_other)) != 0 )
  {
    printf("Connect");
    exit(1);
  }


      if (send(s, "2", strlen("1") , 0 )==-1)
      {
          die("sendto()");
      }


      //send the message
      printf("data--:%s\n",buf);
      if (send(s, buf, strlen(buf) , 0 )==-1)
      {
          die("sendto()");
      }

      //receive a reply and print it
      //clear the buffer by filling null, it might have previously received data
      bzero(buf1, BUFLEN);
      //try to receive some data, this is a blocking call
      if (recv(s, buf1, BUFLEN, 0) == -1)
      {
          die("recvfrom()");
      }

      strcpy(buf,buf1);

  close(s);

  return buf;

}

char * requestSN3(char * buf){

  struct sockaddr_in si_other, si_me;
  int s, i, slen=sizeof(si_other),k;
  char buf1[1024];
  char message[BUFLEN];
  srand(time(NULL));
  //printf("da: %s",buf1);
  k = (rand() % 40000) + 2000 ;
  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
      die("socket");
  }


  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(k);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  //bind socket to port

  if ( bind(s, (struct sockaddr *)&si_me, sizeof(si_me)) != 0 )
	{
		printf("Bind");
		exit(1);
	}
  printf("\nData about to send:%s\n",buf);




  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(port3);

  if (inet_aton(SERVER , &si_other.sin_addr) == 0)
  {
      printf("inet_aton failed\n");
      exit(1);
  }


  if ( connect(s, (struct sockaddr *)&si_other, sizeof(si_other)) != 0 )
  {
    printf("Connect");
    exit(1);
  }


      if (send(s, "2", strlen("1") , 0 )==-1)
      {
          die("sendto()");
      }


      //send the message
      printf("data--:%s\n",buf);
      if (send(s, buf, strlen(buf) , 0 )==-1)
      {
          die("sendto()");
      }

      //receive a reply and print it
      //clear the buffer by filling null, it might have previously received data
      bzero(buf1, BUFLEN);
      //try to receive some data, this is a blocking call
      if (recv(s, buf1, BUFLEN, 0) == -1)
      {
          die("recvfrom()");
      }

      strcpy(buf,buf1);

  close(s);

  return buf;

}
