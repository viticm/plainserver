#include "message/delegator/account.h"
#include "common/net/packets/login_togateway/askauth.h"

using namespace ps_common_net::packets::login_togateway;

uint32_t AskAuthHandler::execute(AskAuth *packet, 
                                 ps_common_net::connection::Base *connection) {
  __ENTER_FUNCTION
    message::delegator::account::auth(packet, connection); 
    return kPacketExecuteStatusContinue;
  __LEAVE_FUNCTION
    return kPacketExecuteStatusError;
}
