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

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
int n=0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    char recvBuff[1024]={0};
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 
    printf("Server is wakeup\n");
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        ticks = time(NULL);
          while ( (n = read(connfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
          printf("Data is received %d\n",n);
        recvBuff[n] = 0;
        int i;
        for(i=0;i<n;i++)
           {
               printf("data[%d]=%x\n",i,recvBuff[i]);
           }

 


    } 

       // close(connfd);
        sleep(1);
     }
}
