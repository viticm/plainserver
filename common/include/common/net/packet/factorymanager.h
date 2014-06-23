/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id factorymanager.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/22 14:16
 * @uses net packet factory manager
 */
#ifndef PS_COMMON_NET_PACKET_FACTORYMANAGER_H_
#define PS_COMMON_NET_PACKET_FACTORYMANAGER_H_

#include "common/sys/thread.h"
#include "common/base/singleton.h"
#include "common/net/config.h"
#include "common/net/packet/factory.h"

namespace ps_common_net {

namespace packet {

class FactoryManager : public ps_common_base::Singleton<FactoryManager> {

 public:
   FactoryManager();
   ~FactoryManager();

 public:
   uint32_t* packet_alloccount_;

 public:
   static FactoryManager& getsingleton();
   static FactoryManager* getsingleton_pointer();
 
 public:
   bool init();
   //根据消息类型从内存里分配消息实体数据（允许多线程同时调用）
   Base* createpacket(uint16_t pakcetid);
   //根据消息类型取得对应消息的最大尺寸（允许多线程同时调用）
   uint32_t getpacket_maxsize(uint16_t packetid);
   //删除消息实体（允许多线程同时调用）
   void removepacket(Base* packet);
   void lock();
   void unlock();
   static bool isvalid_packetid(uint16_t id); //packetid is valid

 private:
   Factory** factories_;
   uint16_t size_;
   ps_common_sys::ThreadLock lock_;

 private:
   void addfactory(Factory* factory);
   void addfactories_for_gatewaylogin();
   void addfactories_for_serverserver();
   void addfactories_for_clientlogin();
   void addfactories_for_logincenter();
   void addfactories_for_servercenter();
   void addfactories_for_clientserver();

};

}; //namespace packet

}; //namespace pap_common_net

extern ps_common_net::packet::FactoryManager* g_packetfactory_manager;

#endif //PS_COMMON_NET_PACKETFACTORY_H_
