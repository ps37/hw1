/*
** server.c -- a stream socket server demo
*/
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define BACKLOG 20 // how many pending connections queue will hold
#define MAXBUFLEN 100
#include "aotp_lib.h"

//global variables
string received_request;
string message;
char operation;
vector<double> operands;
int port_number = 5004;
int numbytes; int numbytes1, numbytes2, numbytes3;
char buf1[MAXBUFLEN];
int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
struct addrinfo hints, *servinfo, *p;
char buf[MAXBUFLEN];
struct sockaddr_storage their_addr; // connector's address information
socklen_t sin_size;
struct sigaction sa;
int yes=1;
char s[INET6_ADDRSTRLEN];
int rv;
string port_number_str;
string result_string;

//error function
    void error(string& s, int socket_int)
    {
        cout<<"ERROR:" + s <<endl;
        s = "ERROR " + s + "\r";
        if ((send(socket_int, s.c_str(), s.size(), 0)) == -1)  //send the error to the client.
           {
               perror("Server Error: Send in the error function");
           }
        cout<<"the server is still listening on:"<<port_number<<endl;
    }
     
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
 if (sa->sa_family == AF_INET) {
 return &(((struct sockaddr_in*)sa)->sin_addr);
 }
 return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
 memset(&hints, 0, sizeof hints);
 hints.ai_family = AF_UNSPEC;   //we can use either ipv6 or ipv4
 hints.ai_socktype = SOCK_STREAM;
 hints.ai_flags = AI_PASSIVE; // use my IP

 if (argc != 3) {
 fprintf(stderr,"usage: server hostname\n");
 exit(1);
 }

//fill in the struct addrinfo with all the info about the server side
 if ((rv = getaddrinfo(NULL, argv[2], &hints, &servinfo)) != 0) {
 fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
 return 1;
 }

// loop through all the results and bind to the first we can
 for(p = servinfo; p != NULL; p = p->ai_next) 
 {
 if ((sockfd = socket(p->ai_family, p->ai_socktype,
 p->ai_protocol)) == -1) {
 perror("server: socket");
 continue;
 }

 if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
 sizeof(int)) == -1) {
 perror("setsockopt");
 exit(1);
 }
 if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
 close(sockfd);
 perror("server: bind");
 continue;
 }
 break;
 }

freeaddrinfo(servinfo);

 if (p == NULL) 
 {
 fprintf(stderr, "server: failed to bind\n");
 return 2;
 }
 else
  printf("server: binded the socket succesfully\n");

 printf("server: waiting for connections... \n");

 while(1) // main accept() loop
  { 
   if (listen(sockfd, BACKLOG) == -1) 
     {
     perror("listen");
     exit(1);
     }

   sin_size = sizeof their_addr;
   new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

   if (new_fd == -1) 
     {
     perror("accept");
     continue;
     }

   inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), 
   s, sizeof s);

   printf("server: client at %s is now connected\n", s);

   if (!fork()) 
     { // this is the child process

     //receive the data form client into a buffer
     //printf("Server: waiting to receive data from the client side....\n"); 
     if ((numbytes = recv(new_fd, buf, MAXBUFLEN-1 , 0)) == -1) 
       {
       perror("recv");
       exit(1);
       }

     //print the adress of the client 
     printf("server: Received packet from client at %s\n",
     inet_ntop(their_addr.ss_family,
     get_in_addr((struct sockaddr *)&their_addr),
     s, sizeof s));
     //print the number of bytes received and print the bytes.
     printf("server: packet is %d bytes long\n", numbytes);
     buf[numbytes] = '\0';
     printf("server: packet contains %s \n", buf);
     
     //reading the message from the client.
    received_request.clear();
    received_request = buf;
    message.clear();
    message = get_message(received_request);
    //cout<<"Message after parsing the received request is:"<<message<<endl;
    void error(string&, int);
    void check_the_message(string&, int);
    check_the_message(message, new_fd);
    }//fork
  }//while
 close(sockfd);
 return 0;
}//main

void check_the_message(string& message, int new_fd)
{
  void change_the_port(string&);
  if(!check_message(message))
            {
                string s = "bad request";
                error(s, new_fd);
            }
            else
            {
                operation = get_operation(received_request);
                if(!check_operation(operation))
                {
                    string s = "invalid operation";
                    error(s, new_fd);
                } 
                else
                {
                   operands.clear();
                   operands = get_operands(received_request);
                   if(!check_operands(operands))
                        {
                            string s = "bad operands";
                            error(s, new_fd);
                        }
                        else
                        {
                            string result_str = do_operation(operation, operands);
                            //cout<<"the result received from the aotp_lib is:"<<result_str<<endl;
                            result_string = "ACR " + result_str + "\r";
                            ostringstream strs;
            		            strs << port_number;
                            port_number_str = strs.str(); 
                            string change_port_msg = "CPN " + port_number_str + "\r";

                            if ((numbytes1 = send(new_fd, change_port_msg.c_str(), change_port_msg.size(), 0)) == -1) 
                            {perror("Server: error while sending the CPN message"); 
                            exit(1);}

                            cout<<"Server: Sent CPN message to the client\n";

                            //receive the change port acknowledgement on the old port(new_fd)
                            if ((numbytes2 = recv(new_fd, buf1, MAXBUFLEN-1 , 0)) == -1) 
                             {
                             printf("error while receiving the CPN_ack");
                             exit(1);
                             }
                            buf1[numbytes2] = '\0';
                             string buffer = buf1;
                             message = get_message(buffer);
                              if(message == "CPN_ACK")
                              {
                                cout<<"Sever: Received the acknowledgement for 'CPN' message:"<<message<<endl;
                                change_the_port(port_number_str);
                              }
                              else if(message != "CPN_ACK")
                              {
                                string s = "Server: Received NO acknowledgement for CPN";
                                error(s, new_fd);
                              }
                              close(new_fd);
                              port_number++;
                          }//else    
                        }//else
                      }//esle  
}

void change_the_port(string& port_number_str)
{
      //change of port number to notified port number??
      //1.create structs using get_addrinfo() system call??
      cout<<"Server: changing to the new port number "<<port_number_str<<endl;
      if ((rv = getaddrinfo(NULL, port_number_str.c_str(), &hints, &servinfo)) != 0) 
      {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      //return 1;
      }
      //2.create new socket with new port num using those structs??
      for(p = servinfo; p != NULL; p = p->ai_next) 
       {
       if ((sockfd = socket(p->ai_family, p->ai_socktype,  //same sock_fd as before but it got new fd
       p->ai_protocol)) == -1) {
       perror("server: socket");
       continue;
       }
       if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
       sizeof(int)) == -1) {
       perror("setsockopt");
       exit(1);
       }
      //3.bind the socket to new port number
       if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
       close(sockfd);
       perror("server: bind");
       continue;
       }
       break;
       }
      //listen for new conns
       freeaddrinfo(servinfo);
       if (p == NULL) 
       {
       fprintf(stderr, "server: failed to bind\n");
       //return 2;
       }
       else
       cout<<"Server: succesfully binded to the port on:"<<port_number<<endl; 

       if (listen(sockfd, BACKLOG) == -1) 
       {
       perror("listen");
       exit(1);
       }

       sin_size = sizeof their_addr;
       new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size); //new_fd got over written

       if (new_fd == -1) 
       {
       perror("accept");
       }
       else
       {
       inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), 
       s, sizeof s);

     printf("server: client at %s is now connected", s);
     cout<<"on the new port number:"<<port_number_str<<endl;
////////////////////////////
     if (!fork()) 
       { // this is the child process
       //close(sockfd); // child doesn't need the listener
 
       if ((numbytes1 = send(new_fd, result_string.c_str(), result_string.size(), 0)) == -1) 
      perror("server: Error while sending the result");
      else{
      //cout<<"Server: sent the result "<<result_string; 
      printf("Server: sent the result to the client at %s\n\n",  inet_ntop(their_addr.ss_family,
      get_in_addr((struct sockaddr *)&their_addr),
      s, sizeof s) );}
      }
      cout<<"Server: The server is now listening on port number:"<<port_number<<endl;
    }
      //return 1;
}
