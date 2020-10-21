#ifndef UTEST_UTEST_H_
#define UTEST_UTEST_H_
#include "UTLs/common.h"
#include <string.h>

PUBLIC void UTEST_init ( const char * );
PUBLIC void UTEST_testcase ( const char * description, const int flag );
PUBLIC void UTEST_report();
PUBLIC bool AssertEquals ( const double, const double );
PUBLIC bool AssertIsNULL ( const void * );
PUBLIC bool AssertIsStrSame ( const char * , const char * );
PUBLIC bool AssertIsTrue ( bool );
PUBLIC bool AssertContains ();


#endif /* UTEST_UTEST_H_ */
