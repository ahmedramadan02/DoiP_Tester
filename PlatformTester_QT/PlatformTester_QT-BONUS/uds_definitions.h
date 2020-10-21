#ifndef UDS_DEF_H_
#define UDS_DEF_H_

//DOIP Specific
#define PROTOCOL_VERSION                                0x02
#define INVERSE_PROTOCOL_VERSION                        0xFD

#define DOIP_HEADER_LENGTH                              8
#define DOIP_PAYLOAD_LENGTH                             0x09
#define DIAG_RAW_BUFF_SIZE                              (DOIP_HEADER_LENGTH + DOIP_PAYLOAD_LENGTH)

#define MAX_RESPONSE_COUNT                              8
#define MAX_ROUTINE_CONTROL_OPTIONS_RECORDS             8                               

#define TARGET_ECU_ADDR                                 0x5555
#define SOURCE_ECU_ADDR                                 0xAAAA

//DoIP Payload types
#define ROUTING_ACTIVATION_REQUEST                      0x0005
#define DOIP_DIAG_MSG                                   0x8001
#define DOIP_POSITIVE_RESPONSE_ACK                      0x8002
#define DOIP_NEGATIVE_RESPONSE_ACK                      0x8003

//Actvation request payload lenght
#define ROUTING_ACTIVATION_REQUEST_PAYLOAD_LENGTH       0x0000000B
#define DOIP_DIAG_MSG_PAYLOAD_LENGTH                    0x00000009                 

//Diag Specific
#define ROUTINE_CONTROL_REQ_SERVICE_ID                  0x31
#define ROUTINE_SUBFUNC_START_ROUTINE                   0x01
#define ROUTINE_SUBFUNC_STOP_ROUTINE                    0x02
#define ROUTINE_SUBFUNC_ROUTINE_REQ_RESULTS             0x03
#define CAMERA_START_STOP_ROUTINE_ID                    0x259   //routine id is 601

//Bytes locations definition 
#define ACK_TYPE_BYTE                                   4
#define POSTIVE_ACK_VALUE                               0x71   
#define NEGATIVE_ACK_VALUE                              0x7F

//RoutineControl Negative response codes
typedef enum{
    SUB_FUN_NOT_SUPPORTED           = 0x12,
    INVALID_FORMAT                  = 0x13,
    CONDITIONS_NOT_CORRECT          = 0x22
    //...
}RoutineControlNegResponseCodes;

//Camera Instances
typedef enum{
    CAMERA01 = 0x01,
    CAMERA02 = 0x02,
    CAMERA03 = 0x02,
    CAMERA04 = 0x03
}CameraInstanceId;

//Diag negative response codes
//Table 
typedef enum{

}UDSNegativeResposeCodes;


typedef enum{
    START_FUNC=1,
    STOP_FUNC,
    REQ_RESULTS
}SubFuncType;

#endif
