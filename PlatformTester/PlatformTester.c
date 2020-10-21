#include "PlatformTester.h"
#include <errno.h>
#include <string.h>

#define LOOPBACK_ADDRESS			"127.0.0.1"
#define TAP_LOCAL_ADDRESS			"192.168.7.2"				

#ifdef TEST_LOCAL==ON
#define USED_ADDR				LOOPBACK_ADDRESS
#else
#define USED_ADDR				TAP_LOCAL_ADDRESS			
#endif

char Response[256];
char Routing_Activation[15];
char DoIP_DiagMessage[17];
int sockfd = 0, n = 0;
struct sockaddr_in serv_addr; 
void Tester_init(void)
{
    Routing_Activation[0]=DoIP_Version;
    Routing_Activation[1]=DoIP_Version_inv;
    Routing_Activation[2]=(char)Routing_Activation_Request;
    Routing_Activation[3]=(char)(Routing_Activation_Request>>8);
    Routing_Activation[4]=(char)(Routing_Activation_Request_Length);
    Routing_Activation[5]=(char)(Routing_Activation_Request_Length>>8);
    Routing_Activation[6]=(char)(Routing_Activation_Request_Length>>16);
    Routing_Activation[7]=(char)(Routing_Activation_Request_Length>>24);
    Routing_Activation[8]=(char)(DoIP_TesterSourceAddress);
    Routing_Activation[9]=(char)(DoIP_TesterSourceAddress>>8);
    Routing_Activation[10]=0x00;
    Routing_Activation[11]=0x00;
    Routing_Activation[12]=0x00;
    Routing_Activation[13]=0x00;
    Routing_Activation[14]=0x00;

    DoIP_DiagMessage[0]=DoIP_Version;
    DoIP_DiagMessage[1]=DoIP_Version_inv;
    DoIP_DiagMessage[2]=(char)(0x80);
    DoIP_DiagMessage[3]=(char)(0x01);
    DoIP_DiagMessage[4]=(char)(0x00);
    DoIP_DiagMessage[5]=(char)(0x00);
    DoIP_DiagMessage[6]=(char)(0x00);
    DoIP_DiagMessage[7]=(char)(0x09);
    DoIP_DiagMessage[8]=(char)(0x55);
    DoIP_DiagMessage[9]=(char)(0x55);
    DoIP_DiagMessage[10]=(char)(0xBB);
    DoIP_DiagMessage[11]=(char)(0xBB);
    DoIP_DiagMessage[12]=0x31;
    DoIP_DiagMessage[13]=0x01;
    DoIP_DiagMessage[14]=(0x02);
    DoIP_DiagMessage[15]=(0x59);
    DoIP_DiagMessage[16]=0x01;
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");

    } else{
	printf("Socket file descriptor is created \n");	
	}
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(13400); 
    if(inet_pton(AF_INET, USED_ADDR, (void *) &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
    } 

    printf("Connecting to %s\n",USED_ADDR);
}
void Tester_Connect(void)
{
	int result = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
 	if( result < 0 )
    	{
       		printf("Error: Connect Failed \n");
		printf("Reason: %s\n", strerror(errno));
		exit(0);
    	}else{
		printf("Connected to %s, and PORT: %d \n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));	
	}
	

}
void Tester_SendRoutingActivation(void)
{
      write(sockfd, Routing_Activation,14); 
}
void Tester_StartRoutine(unsigned char instanceID)
{
      DoIP_DiagMessage[13]=0x01;
      DoIP_DiagMessage[16]=instanceID;
      if(write(sockfd, DoIP_DiagMessage,sizeof(DoIP_DiagMessage)) > 0){
		printf("Written handleStart (0x01) with length: %d \n", sizeof(DoIP_DiagMessage));
	}else{
		printf("Failed to write handleStart (0x01)\n");
	} 

}
void Tester_StopRoutine(unsigned char instanceID)
{
      DoIP_DiagMessage[13]=0x02;
      DoIP_DiagMessage[16]=instanceID;
      if (write(sockfd, DoIP_DiagMessage,sizeof(DoIP_DiagMessage)) > 0){
		printf("Written handle stop (0x02) with length %d: \n", sizeof(DoIP_DiagMessage));
	}else{
		printf("Failed to write handle stop (0x02)\n");
	} 
}

char readResponse(){
    //TODO: TO handle also the pending responses
    //In case the camera was not responding for a long time
    printf("Getting response ...\n");
    
    if((n=read(sockfd, Response, sizeof(Response) - 1)) > 0){
        Response[n] = 0;
        
        if(Response[3] == 0x01){
            if(Response[12] == 0x71)
                printf("Positive response received\n");
            else if (Response[12] == 0x7F)
                printf("negative response received, with NRC: %x\n", Response[14]);
        }else{
            for (size_t i = 0; i < n; i++)
            {
                printf("%x\n", (char)Response[i]);
            }
        }
        
    }else{
        printf("%s\n", strerror(errno));
    }

    n = 0;

	return Response[12];
}

