#ifndef ERR_DEF_H_
#define ERR_DEF_H_

#include <map>
#include <stdint.h>
namespace geeds {
namespace gdpd_gp {

typedef enum ErrClass{
    Error       = 1,
    Info        = 2,
    Fatal       = 3,
    Warning     = 4
}ErrorClass;


class ErrorType{
public:
    ErrorClass _class;
    std::string _category;
    uint8_t _errCode;
    std::string errorMsg;
};

const std::map<std::string, ErrorType> _errors {
    { "COPY_FAILED", {ErrClass::Error, "Memory Issue", 1,"Can't copy data" } },
    { "NETWORK_CONN", {ErrClass::Error, "Network issue", 2,"Can't connect to the socket" } },
    { "SOCKET_READ", {ErrClass::Warning, "Network issue", 3,"Read failed from the server" } },
    { "SOCKET_WRITE", {ErrClass::Warning, "Network issue", 4,"write to the server failed" } },
    { "DISCONNECT", {ErrClass::Error, "Network issue", 4,"Can't disconnect from the server" } },
    { "CLOSED_CONN", {ErrClass::Error, "Network issue", 4,"Connection is closed while sending or receiving!" } },
    { "NO_CONN", {ErrClass::Error, "Network issue", 4,"No connection is established" } }
};


}  // namespace gdpd_gp
}  // namespace geeds

#endif
