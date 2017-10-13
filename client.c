#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <pthread.h>

#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to send data

char username[30];
char filename[33];
int sport;
int myport;


void getStr(char *);
void registerSN(void); //Registering at SuperNode
void logedIN(void); //display menu after login
void chatList(void); // display chat list function
void addFriend(void); //Add a friend function
void chatFriend(char *); //Chat with Selected Friend


void die(char *s)
{
    perror(s);
    exit(1);
}

void AcceptConnections()
{
   long tid;
   struct sockaddr_in si_me, si_other, si_other1;
   int s, i, s1, s2, slen = sizeof(si_other), slen1 = sizeof(si_other1) , recv_len, check;
   char buf[BUFLEN], buf1[BUFLEN], cpy[1024];

   //tid = (long)id;

       //create a UDP socket
       if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
       {
           die("socket");
       }

       // zero out the structure
       memset((char *) &si_me, 0, sizeof(si_me));

       si_me.sin_family = AF_INET;
       si_me.sin_port = htons(myport);
       si_me.sin_addr.s_addr = htonl(INADDR_ANY);

       //bind socket to port
       if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
       {
           die("bind");
       }

       //keep listening for data

   while(1){
     bzero(buf,BUFLEN);
     //try to receive some data, this is a blocking call
     if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
     {
         die("recvfrom()");
     }
     //while(1){
     //bzero(buf,BUFLEN);
     //recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
     //printf("\nMessage from %s:%d :%s\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port),buf);
     printf("\nMessage from Friend:%s\n",buf);

     bzero(buf,BUFLEN);
     printf("\nEnter your Reply:");
     gets(buf);
     sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &si_other, slen);
   //}

   }
   pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
    char uname[20], pass[20],opt[2];
    int sid,port,i;
    char s[1024]={},s1[1024]={};
    struct sockaddr_in ssock,csock;


    if( argc == 4 ) {
        port = atoi(argv[1]);
        sport = atoi(argv[2]);
        myport = atoi(argv[3]);
     }
     else if( argc > 4 ) {
        printf("Too many arguments supplied.\n");
        exit(0);
     }
     else {
        printf("three argument expected.\n");
        exit(0);
    }


    sid=socket(AF_INET,SOCK_STREAM,0);


    ssock.sin_family=AF_INET;
    ssock.sin_addr.s_addr=inet_addr("127.0.0.1");
    ssock.sin_port=htons(port);

    connect(sid,(struct sockaddr *)&ssock,sizeof(ssock));
    while(1)
    {
        printf("Select any one option:\n------------------------\n");
        printf("1. Login\n2. Register\nEnter your option:");
        //gets(opt);
        scanf("%s",opt);


        while(strlen(opt)!=1 || (opt[0]!='1' && opt[0]!='2'))
        {
          printf("\nSelect any one option:\n------------------------\n");
          printf("1. Login\n2. Register\nEnter your option:");
          scanf("%s",opt);
        }

      //  sleep(1);
        if(write(sid,(void*)opt,strlen(opt)))
        {
        if(opt[0]=='1') //login module
        {
          bzero(s,1024);
          printf("Enter username:");
          gets(uname);
          gets(uname);
          printf("Enter password:");
          gets(pass);
          strcpy(username,uname);
          strcat(uname," ");
          strcat(uname,pass);
          if(write(sid,(void*)uname,strlen(uname))){
            read(sid,(void*)s1,sizeof(s1));
            if(s1[0]=='1')
            {
              printf("Success!\n");

              strcpy(filename,username);
              strcat(filename,".txt");
              logedIN();

            }
            else if(s1[0]=='2')
            {
              printf("Failure!\n");
            }
          }
        }
        else if(opt[0]=='2') // register
        {
          bzero(s,1024);
          printf("Enter username:");
          gets(s);
          gets(s);

          if(write(sid,(void*)s,strlen(s))){
              read(sid,(void*)s1,sizeof(s1));
              if(s1[0]=='1')
              {
                printf("Already exists\n");
              }
              else if(s1[0]=='2')
              {
                //new user
                strcpy(username,s);
                strcpy(filename,username);
                strcat(filename,".txt");
                bzero(s,1024);
                printf("Enter password:");
                gets(s);
                write(sid,(void*)s,strlen(s));
                printf("Registered Successfully!\n");
                registerSN();
              }
          }


        }
        }

    }
    close(sid);
}

void getStr(char *str)
{
    int i=0;
    while(str[i]!='\0')
    {
      if(str[i]==' '){
        str[i]='+';
        ++i;
      }
    }
}

void registerSN(){

  struct sockaddr_in si_other;
  int s, i, slen=sizeof(si_other);
  char buf[BUFLEN];
  char message[BUFLEN];



  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
      die("socket");
  }

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(sport);

  if (inet_aton(SERVER , &si_other.sin_addr) == 0)
  {
      fprintf(stderr, "inet_aton() failed\n");
      exit(0);
  }

  while(1)
  {
      if (sendto(s, "3", strlen("1") , 0 , (struct sockaddr *) &si_other, slen)==-1)
      {
          die("sendto()");
      }

      printf("Enter Your UserName IpAddress PortNumber:");
      //sprintf(message,"%s 127.0.0.1 %s",username,myport);
      gets(message);

      //send the message
      if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
      {
          die("sendto()");
      }

      //receive a reply and print it
      //clear the buffer by filling null, it might have previously received data
      bzero(buf, BUFLEN);
      //try to receive some data, this is a blocking call
      if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
      {
          die("recvfrom()");
      }
      if(strcmp(buf,"YES")==0){
        logedIN();
      }
  }

  close(s);
  return;


}


void logedIN(){
  int n;

  int rc;
  long t;
  pthread_t thread;

/*
  rc = pthread_create(&thread, NULL, AcceptConnections, (void *)t);
  if (rc){
       printf("Failed to create thread!\n");
       exit(-1);
       }
*/

  while(1)
  {
    printf("\nSelect One Option:\n------------------\n");
    printf("1.Chat with friends\n2.Add new friends\n3.Idel Mode(Accept Chat Requests)\nEnter your option:");
    scanf("%d",&n);
    if(n==1){
      chatList();
    }
    else if(n==2)
    {
      addFriend();
    }
    else if(n==3)
    {
      AcceptConnections();
    }
  }
  return;
}


void chatList(){
  char cpy[1024],list[50][100];
  int i=0,c;
  FILE *fp;
  fp = fopen(filename, "a+");
  printf("\nSelect friend:\n-------------\n");
  while(fgets(cpy, 1024, fp) != NULL) {
    ++i;
    strcpy(list[i-1],cpy);
		printf("%d. %s",i,cpy);
    bzero(cpy,1024);
  }

  if(i==0)
  {
    printf("Sorry, you have no friends yet!\nTry adding some friends.\n");
  }
  else{
    printf("\n0. Go Back\n");
    printf("Enter your option:");
    scanf("%d",&c);
    if(c==0)
    {
      return;
    }
    else if(c>0 || c<=i){
      printf("\nselected friend: %s\n",list[c-1]);
      chatFriend(list[c-1]);
    }
    else{
      printf("\nInvalid option\n");
    }
  }

  return;
}

void addFriend(){

    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];
    FILE *fp1;

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(sport);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(0);
    }

    //while(1)
    //{
        if(sendto(s, "1", strlen("1") , 0 , (struct sockaddr *) &si_other, slen))
        {
          printf("\nConnected to SuperNode successfully!\n");
          printf("\nEnter Friend UserName:");
          gets(message);
          gets(message);
          printf("\nmessage:%s",message);
        }



        //send the message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            die("sendto()");
        }

        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        bzero(buf, BUFLEN);
        //try to receive some data, this is a blocking call
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen))
        {
            printf("\nreceived:%s\n",buf);
            if(strcmp(buf,"No")!=0)
            {
                fp1 = fopen(filename, "a+");
                if(fprintf(fp1, "%s",buf))
                {
                  printf("\nFriend added successfuly!\n");
                }
                else{
                  printf("\nFailed to add friend. Please try again\n");
                }
                fclose(fp1);
            }
            else{
              printf("\nFailed to add friend. Please try again\n");
            }
        }
        else{
          printf("\nFailed to add friend. Please try again\n");
        }

        //if(strcmp(buf,"YES")==0){

        //  logedIN();
        //}
    //}

    close(s);
    return;


}



/*
void chatFriend(char *name){

      int sid,i;
      char ip[16],port1[6],port[6],name1[20];
      char mssg[1024]={};
      struct sockaddr_in ssock1,csock1;



      strcpy(name1,strtok(name, " "));
      strcpy(ip,strtok(NULL, " "));
      strcpy(port,strtok(NULL, " "));

      printf("name:%s, ip:%s, port:%s",name1,ip,port);




      sid=socket(AF_INET,SOCK_STREAM,0);


      ssock1.sin_family=AF_INET;
      ssock1.sin_addr.s_addr=inet_addr(ip);
      ssock1.sin_port=htons(atoi(port));

      if(connect(sid,(struct sockaddr *)&ssock1,sizeof(ssock1)))
      {
        printf("\nConnected to %s\n",name1);
        while(1)
        {
              bzero(mssg,1024);
              printf("\nEnter Message:");
              gets(mssg);
              if(write(sid,(void*)mssg,strlen(mssg))){
                bzero(mssg,1024);
                read(sid,(void*)mssg,1024);
                printf("Message from %s: %s",name1,mssg);
              }
        }
      }


}

*/


void chatFriend(char * name){

  struct sockaddr_in si_other, si_me;
  int s, i, slen=sizeof(si_other),k;
  char buf1[1024],name1[20],port[6],ip[12];
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

  strcpy(name1,strtok(name, " "));
  strcpy(ip,strtok(NULL, " "));
  strcpy(port,strtok(NULL, " "));


  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(atoi(port));

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

while(1)
{
      //send the message
      printf("\nEnter message:");
      gets(message);
      if (send(s, message, strlen(message) , 0 )==-1)
      {
          die("sendto()");
      }

      if(strcmp(message,"exit")==0 || strcmp(message,"Exit")==0){
        printf("\nChat Ended\n");
        break;
      }

      bzero(message, BUFLEN);
      //try to receive some data, this is a blocking call
      if (recv(s, message, BUFLEN, 0) == -1)
      {
          die("recvfrom()");
      }

      if(strcmp(message,"exit")==0 || strcmp(message,"Exit")==0){
        printf("\nChat Ended\n");
        break;
      }
      else{
        printf("Reply From %s:%s",name1, message);
      }

}

  close(s);

  return;

}
