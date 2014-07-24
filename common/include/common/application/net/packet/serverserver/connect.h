/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id connect.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/23 10:58
 * @uses server connect server packet class
 */
#ifndef PS_COMMON_APPLICATION_NET_PACKET_SERVERSERVER_CONNECT_H_
#define PS_COMMON_APPLICATION_NET_PACKET_SERVERSERVER_CONNECT_H_

#include "common/application/net/packet/serverserver/config.h"
#include "common/net/connection/base.h"
#include "common/net/packet/base.h"
#include "common/net/packet/factory.h"

namespace ps_common_application {

namespace net {

namespace packet {

namespace serverserver {

class Connect : public ps_common_net::packet::Base {

 public:
   Connect();
   virtual ~Connect() {};

 public:  
   virtual bool read(ps_common_net::socket::InputStream& inputstream);
   virtual bool write(ps_common_net::socket::OutputStream& outputstream) const;
   virtual uint32_t execute(ps_common_net::connection::Base* connection);
   virtual uint16_t getid() const;
   virtual uint32_t getsize() const;
   
 public: 
   int16_t get_serverid();
   void set_serverid(int16_t serverid);
   int16_t get_worldid();
   void set_worldid(int16_t worldid);
   int16_t get_zoneid();
   void set_zoneid(int16_t zoneid);

 private:
   int16_t serverid_; //服务器ID
   int16_t worldid_; //世界ID
   int16_t zoneid_; //区域ID

};

class ConnectFactory : public ps_common_net::packet::Factory {

 public:
   ps_common_net::packet::Base* createpacket();
   uint16_t get_packetid() const;
   uint32_t get_packet_maxsize() const;

};

class ConnectHandler {

 public:
   static uint32_t execute(Connect* packet, 
                           ps_common_net::connection::Base* connection);

};

}; //namespace serverserver

}; //namespace packet

}; //namespace net 

}; //namespace ps_common_application

#endif //PS_COMMON_APPLICATION_NET_PACKET_SERVERSERVER_CONNECT_H_
