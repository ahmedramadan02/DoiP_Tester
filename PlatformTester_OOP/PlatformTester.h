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
#include <stdint.h>
#include <sys/select.h>
#include <fcntl.h>
#include <thread>
#include <csignal>
#include <chrono>
#include <atomic>
#include <mutex>

#include "TesterException.h"
#include "uds_definitions.h"
#include "server_definitions.h"
#include "message.h"
#include "PlatformLogger.h"

namespace geeds {
namespace gdpd_gp {

class PlatformTester {
public:
#if defined(SEND_RECEIVE_ANOTHER_THREAD)
void handle_completed();
void init_buffers_async(uint16_t payload_type, uint8_t subfuncType, uint16_t routineIdentifier, uint8_t cameraInstanceId);
#endif

int ConnetToDoIPServer(const char* ip_address, uint16_t port);
void DisconnectFromDoIPServer();

//Only functions that related to DoIP stuff
//The signature of any function should have "int" return
int DoIPRoutingActivationRequest();
int DoIPRoutineControl(uint8_t subfuncType, uint16_t routineIdentifier, uint8_t cameraInstanceId /*,uint8_t option_record[MAX_ROUTINE_CONTROL_OPTIONS_RECORDS]*/);
void SwitchDiagSession(uint8_t sessionId);
int HandleResponse();
bool isConnected() { return _connected; }
void setVerbose(bool _verbose) { isVerbose = _verbose; }
bool getVerbose() { return isVerbose; }

PlatformTester();
~PlatformTester();
#if defined(QT_VERSION)
PlatformTester(QTextEdit* _log);
#endif

void init();

protected:
    virtual void Handle_NAck();
    virtual void Handle_PAck();

private:

void SendDoIPMsg(const DoIPMessage& msg) /*const*/;
DoIPMessage& RecvDoIPMsg();

//Network coversion methods
DoIPMessage& ntoh_prepare(DoIPMessage& _msg);

//Socket functions
int socket_connet(const char* ip_address, uint16_t port);
int socket_write(const uint8_t* ptr, size_t buffer_size);
int socket_read(const uint8_t* ptr, size_t buffer_size);
void cleanup();
void printMsg(const DoIPMessage& _msg) const;

//Non- blocking connect
int nonblocking_connect(int _sockfd, const sockaddr *saptr, socklen_t salen, int nsec);

//Private Vars
//[REQ_DIAG_0006] [REQ_DIAG_0007]
const uint32_t targetECUAddr = (uint32_t) TARGET_ECU_ADDR;
const uint32_t sourceECUAddr = (uint32_t) SOURCE_ECU_ADDR;
int sockfd = 0, n = 0;

//MessageBuffer
DoIPMessage _request, _response;

uint8_t sendBuff[MAX_BUFF_SIZE], recvBuff[MAX_BUFF_SIZE];
struct sockaddr_in serv_addr;  
bool _connected = false;
bool isVerbose = false;

size_t recv_payload_size = 0, sent_payload_size = 0;

//PLogger
Logger _logger;

//TODO: USE QtThread instead, or thread in c++
#if defined(SEND_RECEIVE_ANOTHER_THREAD)
typedef void (*pfunc)();
//async send recive operations
void send_receive_thread(void* params);
void init_async();
void fill_received_async();
int send_async();
int receive_async();

//Send flag
bool _send_flag = false;
//std::thread srThread;
std::unique_ptr<std::thread> srThread;
std::mutex s_mutex;
#endif
};

}  // namespace gdpd_gp
}  // namespace geeds

#endif
