#include "common/net/packet/factorymanager.h"
#include "common/net/packet/base.h"
#include "common/net/packets/id/all.h"

/* packets { */
#include "common/net/packets/serverserver/connect.h"
#include "common/net/packets/gateway_tologin/resultauth.h"
#include "common/net/packets/login_togateway/askauth.h"
/* } packets */

#include "common/net/packet/factorymanager.h"

ps_common_net::packet::FactoryManager* g_packetfactory_manager = NULL;

template<> 
ps_common_net::packet::FactoryManager 
  *ps_common_base::Singleton<
  ps_common_net::packet::FactoryManager>::singleton_ = NULL;

namespace ps_common_net {

namespace packet {

FactoryManager* FactoryManager::getsingleton_pointer() {
  return singleton_;
}

FactoryManager& FactoryManager::getsingleton() {
  Assert(singleton_);
  return *singleton_;
}

FactoryManager::FactoryManager() {
  __ENTER_FUNCTION
    using namespace ps_common_net::packets::id;
    factories_ = NULL;
    size_ = 0;
    size_ = serverserver::kLast - serverserver::kFirst; //common for server

#if defined(_PS_NET_GATEWAY) || defined(_PS_NET_LOGIN)
    size_ += gatewaylogin::kLast - gatewaylogin::kFirst - 1;
    size_ += gateway_tologin::kLast - gateway_tologin::kFirst - 1;
    size_ += login_togateway::kLast - login_togateway::kFirst - 1;
#endif

    Assert(size_ > 0);
    factories_ = new Factory * [size_];
    Assert(factories_);
    packet_alloccount_ = new uint32_t[size_];
    Assert(packet_alloccount_);
    uint16_t i;
    for (i = 0; i < size_; ++i) {
      factories_[i] = NULL;
      packet_alloccount_[i] = 0;
    }
  __LEAVE_FUNCTION
}

FactoryManager::~FactoryManager() {
  __ENTER_FUNCTION
    Assert(factories_ != NULL);
    uint16_t i;
    for (i = 0; i < size_; ++i) {
      SAFE_DELETE(factories_[i]);
    }
    SAFE_DELETE(packet_alloccount_);
  __LEAVE_FUNCTION
}
bool FactoryManager::init() {
  __ENTER_FUNCTION
    addfactories_for_serverserver();
    addfactories_for_gatewaylogin();
    addfactories_for_logincenter();
    addfactories_for_clientlogin();
    addfactories_for_servercenter();
    addfactories_for_clientserver();
    return true;
  __LEAVE_FUNCTION
    return false;
}

Base* FactoryManager::createpacket(uint16_t packetid) {
  __ENTER_FUNCTION
    if (NULL == factories_[packetid]) {
      Assert(false);
      return NULL;
    }
    Base* packet = NULL;
    lock();
    try {
      packet = factories_[packetid]->createpacket();
      ++(packet_alloccount_[packetid]);
    }
    catch(...) {
      packet = NULL;
    }
    unlock();
    return packet;
  __LEAVE_FUNCTION
    return NULL;
}

uint32_t FactoryManager::getpacket_maxsize(uint16_t packetid) {
  __ENTER_FUNCTION
    uint32_t result = 0;
    if (NULL == factories_[packetid]) {
      char temp[FILENAME_MAX] = {0};
      snprintf(temp, 
               sizeof(temp) - 1, 
               "packetid: %d not register in factory!", 
               packetid);
      AssertEx(false, temp);
      return result;
    }
    lock();
    result = factories_[packetid]->get_packet_maxsize();
    unlock();
    return result;
  __LEAVE_FUNCTION
    return 0;
}

void FactoryManager::removepacket(Base* packet) {
  __ENTER_FUNCTION
    if (NULL == packet) {
      Assert(false);
      return;
    }
    uint16_t packetid = packet->getid();
    lock();
    try {
      SAFE_DELETE(packet);
      --(packet_alloccount_[packetid]);
    }
    catch(...) {
      unlock();
    }
    unlock();
  __LEAVE_FUNCTION
}

void FactoryManager::lock() {
  lock_.lock();
}

void FactoryManager::unlock() {
  lock_.unlock();
}

void FactoryManager::addfactory(Factory* factory) {
  __ENTER_FUNCTION
    if (factories_[factory->get_packetid()] != NULL) {
      Assert(false);
      return;
    }
    factories_[factory->get_packetid()] = factory;
  __LEAVE_FUNCTION
}

void FactoryManager::addfactories_for_gatewaylogin() {
#if defined(_PS_NET_GATEWAY) || defined(_PS_NET_LOGIN) /* { */
  __ENTER_FUNCTION
    using namespace ps_common_net::packets;
    addfactory(new gateway_tologin::ResultAuthFactory());
    addfactory(new login_togateway::AskAuthFactory());
  __LEAVE_FUNCTION
#endif /* } */
}

void FactoryManager::addfactories_for_clientlogin() {
#if defined(_PS_NET_LOGIN) || defined(_PS_NET_CLIENT)

#endif
}

void FactoryManager::addfactories_for_logincenter() {
#if defined(_PS_NET_LOGIN) || defined(_PAP_NET_CENTER)

#endif
}

void FactoryManager::addfactories_for_servercenter() {
#if defined(_PS_NET_SERVER) || defined(_PS_NET_CENTER)

#endif
}

void FactoryManager::addfactories_for_clientserver() {
#if defined(_PS_NET_CLIENT) || defined(_PS_NET_SERVER)

#endif
}

void FactoryManager::addfactories_for_serverserver() {
  __ENTER_FUNCTION
    using namespace ps_common_net::packets;
    addfactory(new serverserver::ConnectFactory());
  __LEAVE_FUNCTION
}

bool FactoryManager::isvalid_packetid(uint16_t id) {
  bool result = false;
  __ENTER_FUNCTION
    using namespace ps_common_net::packets::id;
#if defined(_PS_NET_GATEWAY) /* { */
    result = (serverserver::kFirst < id && id < serverserver::kLast) ||  
             (gatewaylogin::kFirst < id && id < gatewaylogin::kLast) ||
             (gateway_tologin::kFirst < id && id < gateway_tologin::kLast) || 
             (login_togateway::kFirst < id && id < login_togateway::kLast);
#elif defined(_PS_NET_LOGIN) /* }{ */

#elif defined(_PS_NET_CENTER) /* }{ */

#elif defined(_PAP_NET_SERVER) /* }{ */

#elif defined(_PAP_NET_CLIENT) /* }{ */

#endif /* } */
  __LEAVE_FUNCTION
    return result;
}

} //namespace packet

} //namespace ps_common_net
