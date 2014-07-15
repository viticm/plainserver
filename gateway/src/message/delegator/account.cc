#include "common/net/packets/gateway_tologin/resultauth.h"
#include "common/base/log.h"
#include "db/user.h"
#include "message/delegator/account.h"

using namespace message::delegator;

void account::auth(ps_common_net::packets::login_togateway::AskAuth *message,
                   ps_common_net::connection::Base *connection) {
  __ENTER_FUNCTION
    db::user_t userinfo = db::user::get_fullinfo(message->getaccount());
    uint8_t result = 1;
    if (strcmp(userinfo.name, message->getpassword())) {
      result = 0;
    }
    ps_common_net::packets::gateway_tologin::ResultAuth *resultmessage;
    resultmessage->setresult(result);
    resultmessage->setaccount(message->getaccount());
    resultmessage->setpassword(message->getpassword());
    connection->sendpacket(resultmessage);
    SLOW_LOG("gateway", 
             "[message.delegator] (account::auth) success!"
             " account: %s, result: %d", 
             message->getaccount(), 
             result);    
  __LEAVE_FUNCTION
}