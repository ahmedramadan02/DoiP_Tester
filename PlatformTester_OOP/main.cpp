#include <iostream>
#include <string>
#include "TesterException.h"
#include "PlatformTester.h"

#include "uds_definitions.h"
#include "server_definitions.h"

//Testing
#include "UTest/UTest.h"


void doTests();

int main (int argc, char** argv){
/* No Threads for now, as No GUI!
    pthread_t cthreadId;
    pthread_create(&cthreadId,NULL,clientThread, NULL);
*/
using ptester =  geeds::gdpd_gp::PlatformTester;
using tester_exception = geeds::gdpd_gp::TesterException;
ptester platform_tester;
int input, instanceId;

    
try{
    //init
    platform_tester.ConnetToDoIPServer(SERVER_ADDRESS, TCP_PORT);

    // The routing activation request is needed to initiate the conversation 
    // between the tester and the DoIP entity. This request is not required 
    // in the DoIP implementation of the APD
    //platform_tester.DoIPRoutingActivationRequest();

    while(1) {
        std::cout << "Enter 1 to start camera routine or 2 to stop camera routine: " << std::endl;
        std::cin >>  input;
        std::cout << "Enter the instace ID: " << std::endl;
        std::cin >> instanceId;
        std::cout << "You entered instance ID: " << instanceId << ", and subroutine: " << input << std::endl;  

        platform_tester.DoIPRoutineControl(input, CAMERA_START_STOP_ROUTINE_ID, instanceId);

    }

    platform_tester.DisconnectFromDoIPServer();

} catch (const tester_exception& e) {  //it's allows a good practice to have catch a specific excpetion
        std::cout << e.what() << std::endl; 
}

/*
    pthread_join(cthreadId, NULL);
*/
    return 0;
}

void doTests() {
	//Init a new test
	UTEST_init( ":::: Tester vs Diag Tests :::" );

	//Get Report
	UTEST_report();

}
