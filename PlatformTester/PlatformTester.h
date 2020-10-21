#ifndef PLATFORM_TESTER_H_
#define PLATFORM_TESTER_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#define DoIP_Generic_Header_Length (0x08)
#define DoIP_Version (0x02)
#define DoIP_Version_inv (0xFD)
#define DoIP_TesterSourceAddress (0xAAAA)
//#define DoIP_ECUAddress (0x5555)
#define DoIP_ECUAddress (0xBBBB)
#define Camera_Routine (601)
#define Routing_Activation_Request (0x0005)
#define Routing_Activation_Request_Length (0x0000007)
#define DiagMessage (0x8001)
#define DiagMessage_Length (0x0000009)
void Tester_init(void);
void Tester_Connect(void);
void Tester_SendRoutingActivation(void);
void Tester_StartRoutine(unsigned char);
void Tester_StopRoutine(unsigned char);



#endif
