/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id resultauth.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/07/10 19:11
 * @uses net packets module gateway to login result auth packet class
 */
#ifndef PS_COMMON_APPLICATION_NET_PACKET_GATEWAY_TOLOGIN_RESULTAUTH_H_
#define PS_COMMON_APPLICATION_NET_PACKET_GATEWAY_TOLOGIN_RESULTAUTH_H_

#include "common/application/net/packet/gateway_tologin/config.h"
#include "common/net/connection/base.h"
#include "common/net/packet/base.h"
#include "common/net/packet/factory.h"
#include "common/application/define/macros.h"

namespace ps_common_application {

namespace net {

namespace packet {

namespace gateway_tologin {

class ResultAuth : public ps_common_net::packet::Base {

 public:
   ResultAuth();
   virtual ~ResultAuth() {};

 public:  
   virtual bool read(ps_common_net::socket::InputStream& inputstream);
   virtual bool write(ps_common_net::socket::OutputStream& outputstream) const;
   virtual uint32_t execute(ps_common_net::connection::Base *connection);
   virtual uint16_t getid() const;
   virtual uint32_t getsize() const;
   
 public: 
   const char *getaccount();
   void setaccount(const char *account);
   const char *getpassword();
   void setpassword(const char *password);
   void setresult(uint8_t result);
   uint8_t getresult() const;

 private:
   char account_[ACCOUNT_LENGTH_MAX];
   char password_[PASSWORD_LENGTH_MAX];
   uint8_t result_;

};

class ResultAuthFactory : public ps_common_net::packet::Factory {

 public:
   ps_common_net::packet::Base *createpacket();
   uint16_t get_packetid() const;
   uint32_t get_packet_maxsize() const;

};

class ResultAuthHandler {

 public:
   static uint32_t execute(ResultAuth *packet, 
                           ps_common_net::connection::Base *connection);

};

}; //namespace gateway_tologin

}; //namespace packet

}; //namespace net

}; //namespace ps_common_application

#endif //PS_COMMON_APPLICATION_NET_PACKET_GATEWAY_TOLOGIN_RESULTAUTH_H_
