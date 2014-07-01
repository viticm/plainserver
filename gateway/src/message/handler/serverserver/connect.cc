#include "common/base/log.h"
#include "common/net/packets/serverserver/connect.h"

using namespace ps_common_net::packets::serverserver;
uint32_t ConnectHandler::execute(Connect* packet, 
                                 ps_common_net::connection::Base* connection) {
  __ENTER_FUNCTION
    USE_PARAM(connection);
    FAST_LOG(kGatewayLogFile,
             "[gateway][message][handler][serverver] (ConnectHandler::execut) success"
             " server id: %d",
             packet->get_serverid());
    return kPacketExecuteStatusContinue;
  __LEAVE_FUNCTION
    return kPacketExecuteStatusError;
}