#include "message/delegator/account.h"
#include "common/net/packets/gateway_tologin/resultauth.h"

using namespace ps_common_net::packets::gateway_tologin;

uint32_t ResultAuthHandler::execute(ResultAuth *packet, 
                                    ps_common_net::connection::Base *connection) {
  __ENTER_FUNCTION
    USE_PARAM(packet);
    USE_PARAM(connection);
    return kPacketExecuteStatusContinue;
  __LEAVE_FUNCTION
    return kPacketExecuteStatusError;
}
