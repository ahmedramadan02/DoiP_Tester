#ifndef UDS_MSG_H_
#define UDS_MSG_H_

#include <stdint.h>
#include "uds_definitions.h"

namespace geeds {
namespace gdpd_gp
{

//Refer to table 27
// DoIP header structure; |PROTOCOL_VERSION|INVERSE_PROTOCOL_VERSION|PAYLOAD_TYPE|PAYLOAD_LENGHT|PAYLOAD|
// Payload Format;  |SOURCE_ECU_ADDR|TARGET_ECU_ADDR|USER_DATA|

// UserData is UDS Request: Table 26
// |SOUREC_ECU||DEST_ECU||USER_DATA|

//TODO: Make it a class and override the << and copy constructor
typedef struct
{
    //Refer to table table 11
    //Header
    uint8_t protocol_version;
    uint8_t inverse_protocol_version;
    uint16_t payload_type;
    uint32_t payload_length;
    
    //Payload
    uint8_t payload[DOIP_PAYLOAD_LENGTH];

}DoIPMessage;



}  // namespace gdpd_gp
}  // namespace geeds

#endif
