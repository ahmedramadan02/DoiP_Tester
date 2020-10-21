#include "UTest.h"
#include <stdio.h>

const static char * _test_name = NULL;
const static char* passtext = "test case passed";
const static char* failtext = "test case failed";

static unsigned long int _pass_count = 0;
static unsigned long int _fail_count = 0;

PUBLIC void UTEST_init ( const char * testname ) {
	_test_name = testname;
	_pass_count = 0;
	_fail_count = 0;
}

PUBLIC void UTEST_testcase ( const char * description, const int flag ) {
	const char * out = "";
	if(flag == true) {
		_pass_count++;
		out = passtext;
	}else{
		_fail_count++;
		out = failtext;
	}

	printf( "%s: =>> %s \n", description, out );
}


PUBLIC void UTEST_report() {
	printf("\n\nReport for %s \n", _test_name);
	printf("Total number of test cases: %ld\n", ( _pass_count +  _fail_count) );
	printf("Number of failed test cases: %ld, number of succeeded test cases %ld\n",
			_fail_count, _pass_count);
}


PUBLIC bool AssertEquals ( const double expected, const double actual  ) { return expected == actual; }
PUBLIC bool AssertIsNULL ( const void * value ) { return (value == NULL) ? true : false; }
PUBLIC bool AssertIsStrSame ( const char * first, const char * second  ) { return memcmp( first, second, ( (sizeof(char) * strlen(second)) + 1) ) == 0;  }
PUBLIC bool AssertIsTrue ( bool flag ) { return flag == true ? true : false; }
PUBLIC bool AssertContains () { return 0; }
