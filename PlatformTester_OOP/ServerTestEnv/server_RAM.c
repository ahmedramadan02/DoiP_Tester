#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <memory.h>


#define MAX_BUFFER_LEN                  127
#define TERMINATE_BYTE                  0xFF

const char* loopback_address = "127.0.0.1";
const uint16_t port 	     = 5000U;

//Globals
uint8_t sendBuff[MAX_BUFFER_LEN], recvBuff[MAX_BUFFER_LEN];
int listenfd = 0, connfd = 0;
struct sockaddr_in serv_addr; 
time_t ticks; 


//functions decleration
void OpenSocketAndListen(const char*, uint16_t);
void PrintRecvBuff();

int main(int argc, char** argv){
    OpenSocketAndListen(loopback_address, port);
    close(connfd);

    return 0;
}

void OpenSocketAndListen(const char* ip, uint16_t port){
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));
    memset(recvBuff, '0', sizeof(recvBuff));

    //Prepare the file descriptor for a socket stream
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    //Prepare the server info
    serv_addr.sin_family = AF_INET;
    //serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //listen to any
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 	
    //bind them together
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
    
    //Start listening and look for connections
    //Accept only one connection from the listen queue
    //accept() shall block until a connection is present
    listen(listenfd, 10);
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
    printf("Connected on %s, and port: %d \n", ip, port);

    while(1)
    {
        ticks = time(NULL);

        //to write to client 
        //snprintf(buff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        //write(connfd, sendBuff, strlen(sendBuff)); 

        //read
        read(connfd, recvBuff, sizeof((recvBuff)-1) );
        if(recvBuff[0] == (uint8_t) TERMINATE_BYTE)
            //exit(0);
            break;
        else
            PrintRecvBuff();

        sleep(1);
     }
}

void PrintRecvBuff(){
    for (size_t i = 0; i < MAX_BUFFER_LEN; i++)
        printf("%x\n", recvBuff[i]);
}
