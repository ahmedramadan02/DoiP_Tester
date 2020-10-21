#include "ApplicationManagement.h"

namespace geeds {
namespace gdpd_gp {

void app_management::init(){
    struct sigaction tAction;
    sigemptyset(&tAction.sa_mask);
	tAction.sa_flags = SA_SIGINFO;
	tAction.sa_sigaction = app_management::terminationHandler;
    setAction(SIGINT, tAction);
}


//Signals
void app_management::terminationHandler(int, siginfo_t*, void*){
    geeds::gdpd_gp::terminationflag = 1;
    printf("Received termination interrupt\n");
    StaticCleanUp();//Cleanup hook
} 

int app_management::setAction(int _signalType, struct sigaction _action){
	// set action to catch a signal (timer, interrupt)
	if (sigaction(_signalType, &_action, NULL) == -1) {
		perror("sigaction failed");
		return -1;
	}
}

void app_management::StaticCleanUp(){

}

}  // namespace gdpd_gp
}  // namespace geeds
