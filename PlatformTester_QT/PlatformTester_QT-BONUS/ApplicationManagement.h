#ifndef APPLICATION_MANAGEMENT_H_
#define APPLICATION_MANAGEMENT_H_

#include <stdio.h>
#include <signal.h>
#include <stdint.h>

namespace geeds {
namespace gdpd_gp {

//Global Vars for signals
static uint8_t terminationflag = 0;

class app_management {
public:
//Definitions for the signals
static void init();
static void terminationHandler(int, siginfo_t*, void*);
static void StaticCleanUp(); 
static int setAction(int, struct sigaction);

};

}  // namespace gdpd_gp
}  // namespace geeds

#endif
