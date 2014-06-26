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
#ifndef PS_COMMON_NET_PACKETS_SERVERSERVER_CONNECT_H_
#define PS_COMMON_NET_PACKETS_SERVERSERVER_CONNECT_H_

#include "common/net/packets/serverserver/config.h"
#include "common/net/connection/base.h"
#include "common/net/packet/base.h"
#include "common/net/packet/factory.h"

namespace ps_common_net {

namespace packets {

namespace serverserver {

class Connect : public packet::Base {

 public:
   Connect();
   virtual ~Connect() {};

 public:  
   virtual bool read(socket::InputStream& inputstream);
   virtual bool write(socket::OutputStream& outputstream) const;
   virtual uint32_t execute(connection::Base* connection);
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

class ConnectFactory : public packet::Factory {

 public:
   packet::Base* createpacket();
   uint16_t get_packetid() const;
   uint32_t get_packet_maxsize() const;

};

class ConnectHandler {

 public:
   static uint32_t execute(Connect* packet, connection::Base* connection);

};

}; //namespace serverserver

}; //namespace packets

}; //namespace ps_common_net

#endif //PS_COMMON_NET_PACKETS_SERVERSERVER_CONNECT_H_
