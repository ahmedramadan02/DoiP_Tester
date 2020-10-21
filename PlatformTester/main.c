#include <stdio.h>
#include "PlatformTester.h"
#include "UTest/UTest.h"

#define DO_TESTS

void doTests();

int main (int argc, char** argv)
{
unsigned char input =0;
unsigned char instaceID=0;
 Tester_init();
 Tester_Connect();
 //Tester_SendRoutingActivation();

#ifdef DO_TESTS
 doTests();
#endif 

while(1)
 {
   printf("Enter 1 to start camera routine or 2 to stop camera routine: ");
   scanf("%d",&input);
   printf("Enter the instace ID: ");
   scanf("%d",&instaceID);

   printf("You entered instance ID: %d, and subroutine: %d \n", instaceID, input );  

/*
   if(instaceID>4) {
	printf("Invalid instace ID\n");
   }
*/
   if(input==1) {
      Tester_StartRoutine(instaceID);
      printf("Waiting for response\n");
      readResponse(); //DoIP ACK
      readResponse(); // DIAG Response
     }
   else if(input==2) {
       Tester_StopRoutine(instaceID);
       printf("Waiting for response");
      readResponse(); //DoIP ACK
      readResponse(); // DIAG Response
    }else{
      printf("you entered a not valid values \n");
    }

 }
    return 0;
}

void doTests() {
	//Init a new test
	UTEST_init( ":::: Tester vs Diag Tests :::" );

   char twelfth;

	//Test01
   //Scenario: 
	const uint8_t expectedData01 = 0x71;
   Tester_StartRoutine(0x01);
   readResponse();
   twelfth = readResponse();
   UTEST_testcase( "#01 Test send in range camera instance ", AssertEquals(twelfth, expectedData01) );
   
	
	//Test02
   //Scenario: 
   const uint8_t expectedData02 = 0x7F;
   Tester_StartRoutine(0x06);
   readResponse();
   twelfth = readResponse();
	UTEST_testcase( "#01 Test send out of range instance id ", AssertEquals(twelfth, expectedData02) );

	//Get Report
	UTEST_report();

}
