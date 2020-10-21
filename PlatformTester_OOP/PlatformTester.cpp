#include "PlatformTester.h"
#include "uds_definitions.h"
#include "message.h"
#include "server_definitions.h"
#include "errors_definitions.h"

#include "ApplicationManagement.h"

#include "stdio.h"
#include <iostream>
#include <exception>

namespace geeds {
namespace gdpd_gp {

//Global for signals
#if defined(SEND_RECEIVE_ANOTHER_THREAD)
PlatformTester* tempPtr;
static void wrapperHandle_completed(int signo);
#endif


//The constructor
PlatformTester::PlatformTester(){
        this->init();
}
#if defined(QT_VERSION)
PlatformTester::PlatformTester(QTextEdit* _log):_logger(_log) {
        this->init();
}
#endif
PlatformTester::~PlatformTester(){
        cleanup();

#if defined (SEND_RECEIVE_ANOTHER_THREAD)
        srThread->join();
#endif
}

int PlatformTester::DoIPRoutingActivationRequest() {
        //Prepare the message
        //Refer to table 22
        DoIPMessage msg;
        msg.protocol_version = PROTOCOL_VERSION;
        msg.inverse_protocol_version = INVERSE_PROTOCOL_VERSION;
        msg.payload_type = ntohs(ROUTING_ACTIVATION_REQUEST);
        msg.payload_length = ntohl(ROUTING_ACTIVATION_REQUEST_PAYLOAD_LENGTH);
        msg.payload[0]=(uint8_t)(SOURCE_ECU_ADDR>>8); //SOURCE ADDRESS
        msg.payload[1]=(uint8_t)(SOURCE_ECU_ADDR); 
        msg.payload[3]=(uint8_t)(0x01); //ACTIVATION type (unknown)
        msg.payload[4]=0x00;
        msg.payload[5]=0x00;
        msg.payload[6]=0x00;
        msg.payload[7]=0x00;
        msg.payload[8]=0x00;
        msg.payload[9]=0x00;
        msg.payload[10]=0x00;
        msg.payload[11]=0x00;

        //send the msg
        this->SendDoIPMsg(msg);

        //handle the response
        HandleResponse(); //DoIP ACK
        return HandleResponse(); //DIAG ACK
}

//TODO: TO handle also the pending responses
//In case the camera was not responding for a long time
//Copy the buffer to our structure, and return the a pointer
int PlatformTester::HandleResponse(){
        this->RecvDoIPMsg();
        printMsg(this->_response);
        if(this->_response.payload_type == ROUTING_ACTIVATION_REQUEST){
                size_t headerSize = recv_payload_size - this->_response.payload_length;
                printMsg(this->_response);

        }else if (this->_response.payload_type == DOIP_DIAG_MSG){
                if(this->_response.payload[ACK_TYPE_BYTE] == POSTIVE_ACK_VALUE){
                        _logger.log(LogType::Info, "%s", "DIAG positive ACK received");
                        Handle_PAck();
                }else{
                        _logger.log(LogType::Info, "%s", "DIAG negative ACK received");
                        Handle_NAck();
                }
        }

        return 1;
}

//int PlatformTester::DoIPRoutineControl(SubFuncType subfuncType, uint16_t routineIdentifier, CameraInstanceId cameraInstanceId /*,uint8_t option_record[MAX_ROUTINE_CONTROL_OPTIONS_RECORDS]*/){ 
int PlatformTester::DoIPRoutineControl(uint8_t subfuncType, uint16_t routineIdentifier, uint8_t cameraInstanceId /*,uint8_t option_record[MAX_ROUTINE_CONTROL_OPTIONS_RECORDS]*/){
        //Prepare the message
        //Refer to table 26
        DoIPMessage msg;
        msg.protocol_version = PROTOCOL_VERSION;
        msg.inverse_protocol_version = INVERSE_PROTOCOL_VERSION;
        msg.payload_type = ntohs(DOIP_DIAG_MSG);
        msg.payload_length = ntohl(DOIP_DIAG_MSG_PAYLOAD_LENGTH);
	//[REQ_DIAG_0006]
        msg.payload[0]=(uint8_t)(SOURCE_ECU_ADDR>>8); //SOURCE ADDRESS
        msg.payload[1]=(uint8_t)(SOURCE_ECU_ADDR); 
        msg.payload[2]=(uint8_t)(TARGET_ECU_ADDR>>8); //SOURCE ADDRESS
        msg.payload[3]=(uint8_t)(TARGET_ECU_ADDR); 
        msg.payload[4]=ROUTINE_CONTROL_REQ_SERVICE_ID;

        switch (subfuncType)
        {
        case SubFuncType::START_FUNC:
                msg.payload[5]=ROUTINE_SUBFUNC_START_ROUTINE;
                break;
        
        case SubFuncType::STOP_FUNC:
                msg.payload[5]=ROUTINE_SUBFUNC_STOP_ROUTINE;
                break;
        case SubFuncType::REQ_RESULTS:
               msg.payload[5]=ROUTINE_SUBFUNC_ROUTINE_REQ_RESULTS;
                break;
        default:
                break;
        }

        //
        msg.payload[6]=(uint8_t)(routineIdentifier>>8);
        msg.payload[7]=(uint8_t)(routineIdentifier);

        //Camera Instance Id
	//[REQ_DIAG_0005] option record has one byte, the camera instance
        msg.payload[8]=cameraInstanceId;


        //send the msg
        this->SendDoIPMsg(msg);

        //handle the response
        HandleResponse(); //DoIP ACK
        return HandleResponse(); //DIAG ACK
}

void PlatformTester::SwitchDiagSession(uint8_t sessionId){

}

//Private members definition
int PlatformTester::ConnetToDoIPServer(const char* ip_address, uint16_t port){
        if(this->socket_connet(ip_address, port) >= 0){
                //Success
                this->_connected = true;
                _logger.log(LogType::Info, "Connected to %s via port: %d", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port) );
        }else{
                _logger.log(LogType::Error, "Couldn't connect to %s via port: %d", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port) );
                throw TesterException(_errors.find("NETWORK_CONN")->second._errCode, 
                        _errors.find("NETWORK_CONN")->second.errorMsg);
        }
}

void PlatformTester::DisconnectFromDoIPServer(){
    if(this->_connected == false){
        throw TesterException(_errors.find("NO_CONN")->second._errCode,
                _errors.find("NO_CONN")->second.errorMsg);
    }

    if(close(sockfd) < 0){
                throw TesterException(_errors.find("DISCONNECT")->second._errCode, 
                        _errors.find("DISCONNECT")->second.errorMsg); 
        }
        this->_connected = false; 
        //Clean up any allocations
        this->cleanup();
}


void PlatformTester::SendDoIPMsg(const DoIPMessage& msg) /*const*/ {
        if(this->_connected == false)
            throw TesterException(_errors.find("CLOSED_CONN")->second._errCode,
            _errors.find("CLOSED_CONN")->second.errorMsg);

        //TODO: Use move sematics
        //size_t msgSize = sizeof(msg)-3;
        if(  memcpy(  (void*) sendBuff, (void*)&msg, DIAG_RAW_BUFF_SIZE) == nullptr ) {
                throw TesterException(_errors.find("COPY_FAILED")->second._errCode, 
                _errors.find("COPY_FAILED")->second.errorMsg);
        }

        // const_cast<PlatformTester*>(this)->sendBuff
        if( socket_write( sendBuff , DIAG_RAW_BUFF_SIZE ) < 1 ){
                throw TesterException(_errors.find("SOCKET_WRITE")->second._errCode, 
                _errors.find("SOCKET_WRITE")->second.errorMsg);
        }
}

DoIPMessage& PlatformTester::RecvDoIPMsg() { 
        int n = this->socket_read( recvBuff , DIAG_RAW_BUFF_SIZE );

        //Form to the DoIP message
        //TODO: Use move sematics
        if( memcpy((void*) &(this->_response), (void*) recvBuff, n) == nullptr ){
                throw TesterException(_errors.find("COPY_FAILED")->second._errCode, 
                _errors.find("COPY_FAILED")->second.errorMsg);
        }

        return ntoh_prepare(this->_response);
}

//Socket functions
int PlatformTester::socket_connet(const char* ip_address, uint16_t port){ 
        int result = 1;
        //1. Open the socket file
        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
                _logger.log(LogType::Error, "couldn't open the socket");
                result = -1;
        }

        //2. Set the server configurations
        memset(&serv_addr, '0', sizeof(serv_addr)); 
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port); 
        //serv_addr.sin_addr.s_addr = inet_addr(ip_address);
        if(inet_pton(AF_INET, ip_address, (void *) &serv_addr.sin_addr)<=0) //revere order inet_ntop()
        {
                _logger.log(LogType::Error, "Wrong Ip address format");
                result = -1;
        }

        //3. bind all together
        //blocking connect
 	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) < 0 ) {
        //Non-blocking connect
        //if( nonblocking_connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr), 3) < 0) {
                result = -1;
        }
        
        return result;
}

int PlatformTester::socket_write(const uint8_t* ptr, size_t buffer_size) {
        if(buffer_size > MAX_BUFF_SIZE){
                _logger.log(LogType::Error, "bad buffer size passed ");
                return -1;
        }

        n = write(sockfd, (void*) ptr, buffer_size); 
        if(n < 0){
                _logger.log(LogType::Error, "write error ");
                return -1;
        } 

        this->sent_payload_size = n; //will be used at the upper layer to get the request payload size if needed
        return n;
}

int PlatformTester::socket_read(const uint8_t* ptr, size_t buffer_size){
        if(buffer_size > MAX_BUFF_SIZE){
                _logger.log(LogType::Error, "bad buffer size passed ");
                return -1;
        }

        n = read(sockfd, (void*) ptr, buffer_size) ;

        if(n < 0){
                _logger.log(LogType::Error, "Read failed ");
        } 

        this->recv_payload_size = n; //used by the upper layer to specify the the payload size
        return n;
}

DoIPMessage& PlatformTester::ntoh_prepare(DoIPMessage& _msg){
        _msg.payload_type = ntohs(_msg.payload_type);
        _msg.payload_length = ntohl(_msg.payload_length); 
}

void PlatformTester::Handle_NAck(){ }

void PlatformTester::Handle_PAck(){ }

void PlatformTester::init(){
        _logger.log(LogType::Info, "%s", "initializing ...");

        memset(recvBuff, '0',sizeof(recvBuff));
        memset(recvBuff, '0',sizeof(sendBuff));
        memset(&serv_addr, '0', sizeof(serv_addr));

#if defined(SEND_RECEIVE_ANOTHER_THREAD)
        init_async();
        tempPtr = this;
#endif

        //Init the platform
        geeds::gdpd_gp::app_management::init();
}

//TODO: << to be overriden for message
void PlatformTester::printMsg(const DoIPMessage& _msg) const {
        _logger.log(LogType::Info, "### Message Header ### ");
        _logger.log(LogType::Info, "Message Type %x", _msg.payload_type);
        _logger.log(LogType::Info, "Payload: ");
        for (size_t i = 0; i < _msg.payload_length - 1; i++)
                _logger.log(LogType::Info, "Payload[%d]: %x ", i, _msg.payload[i]);
}

int PlatformTester::nonblocking_connect(int _sockfd, const sockaddr *saptr, socklen_t salen, int nsec)
{
	int flags, n, error = 0;
	socklen_t len;
	fd_set	rset, wset;
	struct timeval	tval;
        int ret = 0;

	flags = fcntl(sockfd, F_GETFL, 0); //save the flags to restore later
	fcntl(sockfd, F_SETFL, flags | O_NONBLOCK); //set non-blocking

	
	if ( (n = connect(sockfd, (struct sockaddr *) saptr, salen)) < 0){
		if (errno != EINPROGRESS)
			ret = -1;

                /* Do whatever we want while the connect is taking place. */

                FD_ZERO(&rset);
                FD_SET(sockfd, &rset);
                wset = rset;
                tval.tv_sec = nsec;
                tval.tv_usec = 0;

                //ALWAYS Returns 2 as the writable and readable socket
                //So check again if connect returns -1 even if select returns true
                if ( ( (n = select(sockfd+1, &rset, &wset, NULL,
                                                nsec ? &tval : NULL)) == 0) ) {
                        _logger.log(LogType::Error, "Connection timeout!");
                        close(sockfd);
                        errno = ETIMEDOUT;
                        ret = -1;
                }

                if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
                        len = sizeof(error);
                        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
                                ret = -1;
                } else{
                        _logger.log(LogType::Error, "select error: sockfd not set!");
                        ret = -1;
                }
        
        }else if(n == 0){
                /* connect completed immediately */
                ret = 0;
	        fcntl(sockfd, F_SETFL, flags);	/* restore file status flags */

        }

        return ret;
}

void PlatformTester::cleanup(){

}

#if defined(SEND_RECEIVE_ANOTHER_THREAD)
//async operations
//Use two varaibles as siganls and make it generic
//otherwise use QT signals and slots and you should inherite from QObject
//Also we can use std::async
void PlatformTester::send_receive_thread(void* params){
    PlatformTester* pcallback =
            reinterpret_cast<PlatformTester*>(params);
    std::mutex s_mutex;
    while(1){
        if(geeds::gdpd_gp::terminationflag) break;
        if(_send_flag){
            s_mutex.lock();
            send_async();
            receive_async();
            fill_received_async();
            _send_flag = false;
            raise(SIGUSR1);
            s_mutex.unlock();
         }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }
}


void PlatformTester::init_async(){
    //Pipe new thread
    //srThread = std::thread(&geeds::gdpd_gp::PlatformTester::send_receive_thread, this,this);
    srThread = std::make_unique<std::thread>(&geeds::gdpd_gp::PlatformTester::send_receive_thread,
                                             this, this);
    signal(SIGUSR1, wrapperHandle_completed);

    if(srThread)
        _logger.log(LogType::Info, "Thread created for send/receive handling");
    else
        _logger.log(LogType::Error, "unable to create thread");
}

void PlatformTester::init_buffers_async(uint16_t payload_type, uint8_t subfuncType, uint16_t routineIdentifier, uint8_t cameraInstanceId){
    if(0x8001 == payload_type){
        //1. Fill buffers
        //Prepare the message
        //Refer to table 26
        DoIPMessage& msg = this->_request;
        msg.protocol_version = PROTOCOL_VERSION;
        msg.inverse_protocol_version = INVERSE_PROTOCOL_VERSION;
        msg.payload_type = ntohs(DOIP_DIAG_MSG);
        msg.payload_length = ntohl(DOIP_DIAG_MSG_PAYLOAD_LENGTH);
        msg.payload[0]=(uint8_t)(SOURCE_ECU_ADDR>>8); //SOURCE ADDRESS
        msg.payload[1]=(uint8_t)(SOURCE_ECU_ADDR);
        msg.payload[2]=(uint8_t)(TARGET_ECU_ADDR>>8); //SOURCE ADDRESS
        msg.payload[3]=(uint8_t)(TARGET_ECU_ADDR);
        msg.payload[4]=ROUTINE_CONTROL_REQ_SERVICE_ID;

        switch (subfuncType)
        {
        case SubFuncType::START_FUNC:
                msg.payload[5]=ROUTINE_SUBFUNC_START_ROUTINE;
                break;

        case SubFuncType::STOP_FUNC:
                msg.payload[5]=ROUTINE_SUBFUNC_STOP_ROUTINE;
                break;
        case SubFuncType::REQ_RESULTS:
               msg.payload[5]=ROUTINE_SUBFUNC_ROUTINE_REQ_RESULTS;
                break;
        default:
                break;
        }

        //
        msg.payload[6]=(uint8_t)(routineIdentifier>>8);
        msg.payload[7]=(uint8_t)(routineIdentifier);

        //Camera Instance Id
	//[REQ_DIAG_0005] option record has one byte, the camera instance
        msg.payload[8]=cameraInstanceId;

        //2. set the send flag
        _send_flag = true;
    }
}

void PlatformTester::fill_received_async(){

}

int PlatformTester::send_async(){
    //send the msg
    if(_connected){
    this->SendDoIPMsg(this->_request);
    }else{
        this->_logger.log(LogType::Error, "You can only send if the connection open");
    }
}

int PlatformTester::receive_async(){
    //handle the response
    std::mutex r_mutex;
    //r_mutex.lock();
    HandleResponse(); //DoIP ACK
    HandleResponse(); //DIAG ACK
    //r_mutex.unlock();
}


void PlatformTester::handle_completed(){
    this->receive_async();
}

void wrapperHandle_completed(int signo){
    if(signo == SIGUSR1){
        tempPtr->handle_completed();
    }
}

#endif

}  // namespace gdpd_gp
}  // namespace geeds
