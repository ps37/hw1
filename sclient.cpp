/*
** client.c -- a stream socket client demo
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
using namespace std;

//global variables
string message;
string data;
string port_number;
#define MAXDATASIZE 100 // max number of bytes we can get at once

void *get_in_addr(struct sockaddr *sa)
{
 if (sa->sa_family == AF_INET) {
 return &(((struct sockaddr_in*)sa)->sin_addr);
 }
 return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
 int sockfd, sockfd_new, numbytes, numbytes1;
 char buf[MAXDATASIZE];
 struct addrinfo hints, *servinfo, *p;
 int rv;
 char s[INET6_ADDRSTRLEN];

 if (argc != 5) {
 fprintf(stderr,"usage: client hostname\n");
 exit(1);
 }

 memset(&hints, 0, sizeof hints);
 hints.ai_family = AF_UNSPEC;
 hints.ai_socktype = SOCK_STREAM;

 if ((rv = getaddrinfo(argv[4], argv[2], &hints, &servinfo)) != 0) {
 fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
 return 1;
 }

printf("client: connecting to the server at host %s and port %s\n", argv[4], argv[2]);
 // loop through all the results and connect to the first we can
 for(p = servinfo; p != NULL; p = p->ai_next) 
 {
 if ((sockfd = socket(p->ai_family, p->ai_socktype,
 p->ai_protocol)) == -1) {
 perror("client: socket");
 continue;
 }
 if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
 {
 close(sockfd);
 perror("client: connect");
 continue;
 }
 else
    printf("client: connection is successful\n");
 break;
 }

 inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
 s, sizeof s);

 freeaddrinfo(servinfo); // all done with this structure

 if (p == NULL) 
 {
 fprintf(stderr, "client: failed to connect\n");
 return 2;
 }
 
 //take the input from the user
 vector<string> operands;
 string operand;
 string operation;
 string operands_list;
 string request = "AOSR";

 cout<<"please enter the operation you want to perform:\n";
 cout<<"Please enter the phrases in double quotes for choosing corresponding operations:"; 
 cout<<"\"add\" for addition \n";
 cout<<"\"subtract\" for subtraction \n";
 cout<<"\"devide\" for devision \n";
 cout<<"\"multiply\" for multiplication operation. \n\n";
 cin>>operation;
 if(operation == "add")
 request += " +";
 else if(operation == "subtract")
 request += " -";
 else if(operation == "devide")
 request += " /";
 else if(operation == "multiply")
 request += " *";
 else
 request += " ~";

 cout<<"please enter the operands and press \"EOF character\" when you are done entering the operands\n";
 while(cin)
 {
     cin>>operand;
     operands.push_back(operand);
 }

 for (int i = 0; i < operands.size()-1; ++i)
    {   
        operands_list += " ";
        operands_list += operands[i];
    }

 operands_list += "\n";
 request += operands_list; 
  
 //sending the operation request message to the server
  if ((numbytes = send(sockfd, request.c_str(), request.size(), 0)) == -1) {
 perror("client: send");
 exit(1);
 }
 cout<<"client: sent the request of "<<numbytes<<" bytes as:"<<request<<endl;
 //printf("client: sent %d bytes to server at %s\n", numbytes, argv[4]);
 
 //receive the message from the server
 string buf1;
 if ((numbytes1 = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
 perror("recv");
 exit(1);
 }
 //analyze the message by parsing it
 buf[numbytes1] = '\0';
 string buffer = buf;
 printf("%s\n", buf);
 cout<<"Client: The number of received bytes is: "<<numbytes1<<endl;
 cout<<"Client: Received the following message from server:"<<buffer<<endl;
 void parse_the_message(string&);
message.clear();
data.clear();
 parse_the_message(buffer);

 if(message == "CPN") //check if the msg is CPN or Error or Result. message = CPN and data = port.
    {
        port_number = data;
        data.clear();
        //cout<<"Client: received the port number:"<<port_number<<" in the change Port notice CPN message"<<endl;
        message.clear();    //clear the message for using it again.
        string one = "1";
        string CPN_ack = "CPN_ACK " + one + "\r";  
        //send the CPN_ACK for the CPN message.
        if ((numbytes = send(sockfd, CPN_ack.c_str(), CPN_ack.size(), 0)) == -1) //send the CPN ack
        {
         perror("client: send");
         exit(1);
        }
         cout<<"client: sent CPN acknowledgement message to the server as:"<<CPN_ack<<endl;

         cout<<"client: is getting connected to the new port number:"<<port_number<<"...\n";
         //probably add some delay.
         for (double i = 0; i < 200000; i = i + 0.5)
          {

          } 

         //now create new socket for revceiving on the the new port
         if ((rv = getaddrinfo(argv[4], port_number.c_str(), &hints, &servinfo)) != 0) 
         {
         fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
         return 1;
         }
         // loop through all the results and connect to the first one we can
         for(p = servinfo; p != NULL; p = p->ai_next) 
         {
         if ((sockfd_new = socket(p->ai_family, p->ai_socktype,   //creating new socet fd with the new port number
         p->ai_protocol)) == -1) {
         perror("client: socket");
         continue;
         }
         if (connect(sockfd_new, p->ai_addr, p->ai_addrlen) == -1) {  // connecting the new socket fd to the server
         close(sockfd);
         perror("client: connect");
         continue;
         }
         break;
         }
         
         //checking the connection:
         if (p == NULL) 
         {
         fprintf(stderr, "client: failed to connect\n");
         return 2;
         }
         //else

        //receive the 2nd message from server using new socket descriptor
         if ((numbytes1 = recv(sockfd_new, buf, MAXDATASIZE-1, 0)) == -1) 
         {
         perror("recv");
         exit(1);
         }
         else
            fprintf(stderr, "client: connection is successful to the new port number\n");

         buf[numbytes1] = '\0';
         buffer = buf;
         cout<<"Client: received message on the new port is:";
         //printf("%s\n", buf);
         cout<<buffer<<endl<<endl;
         parse_the_message(buffer);
         if (message == "ACR")
            {
                cout<<"**** RESULT:"<<data<<" ****"<<endl;
                printf("\nClient: The server is now listening on the new port number %s", port_number.c_str());
            }

        else if (message == "ERROR")
            {
                cout<<"**** ERROR:"<<data<<" ****"<<endl;
                printf("The server is still listening on the old port number\n");
            }

    }

else if (message == "ACR")
    {
        cout<<"**** RESULT:"<<data<<" ****"<<endl;
        printf("\nClient: The server is now listening on the new port number %s", port_number.c_str());
    }

else if (message == "ERROR")
    {
        cout<<"**** ERROR:"<<data<<" ****"<<endl;
        printf("The server is still listening on the old port number\n");
    }
else
    cout<<"I am in else"<<endl;

<<<<<<< HEAD
||||||| merged common ancestors
printf("\nClient: The server is now listening on the new port number %s", port_number.c_str());
=======
//printf("\nClient: The server is now listening on the new port number %s", port_number.c_str());
>>>>>>> master
cout<<"\n"<<endl;
port_number.clear(); //clear the data for using it again
data.clear();
message.clear();
close(sockfd);
close(sockfd_new);
return 0;
}

 void parse_the_message(string& buffer)
 {
    int rcvd_msg_index = 0;
    while(buffer[rcvd_msg_index]!=' ')   //loop for collecting the message from request
    {
        message += buffer[rcvd_msg_index];
        rcvd_msg_index++;
    }
    cout<<"message is: "<<message<<endl; //CPN message

    rcvd_msg_index = rcvd_msg_index+1;

    while(buffer[rcvd_msg_index]!='\r')
    {
     data += buffer[rcvd_msg_index];
        rcvd_msg_index++; 
    }
    cout<<"data is: "<<data<<endl; //port number
 }

