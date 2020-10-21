#ifndef TESTER_EXCEPTION_H_
#define TESTER_EXCEPTION_H_

#include <string>
#include <exception>
#include "errors_definitions.h"

namespace geeds {
namespace gdpd_gp {

//TODO: Try to inherite from std::runtime_error
class TesterException : public std::exception {
public:
const char *what() const throw(){
    return user_message.c_str();
}

explicit TesterException(int code ,const std::string& msg): 
        errorCode(code), errorMsg(msg){ 
            user_message = "Error# " 
                        + std::to_string(errorCode) 
                        + ": " + errorMsg;
         }
~TesterException() throw () {}

private:
    std::string user_message;
    int errorCode; 
    std::string errorMsg;
};

}  // namespace gdpd_gp
}  // namespace geeds


#endif
