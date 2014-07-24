#include "common/net/packet/base.h"
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
    factorycount_ = 0;
    size_ = 0;
    addfactories_for_clientserver();
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
    extend_forinit();
    return true;
  __LEAVE_FUNCTION
    return false;
}

Base* FactoryManager::createpacket(uint16_t packetid) {
  __ENTER_FUNCTION
    bool isfind = idindexs_.isfind(packetid);
    uint16_t index = idindexs_.get(packetid);
    if (!isfind || NULL == factories_[index]) {
      Assert(false);
      return NULL;
    }
    Base* packet = NULL;
    lock();
    try {
      packet = factories_[index]->createpacket();
      ++(packet_alloccount_[index]);
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
    bool isfind = idindexs_.isfind(packetid);
    uint16_t index = idindexs_.get(packetid);
    if (!isfind || NULL == factories_[index]) {
      char temp[FILENAME_MAX] = {0};
      snprintf(temp, 
               sizeof(temp) - 1, 
               "packetid: %d not register in factory!", 
               packetid);
      AssertEx(false, temp);
      return result;
    }
    lock();
    result = factories_[index]->get_packet_maxsize();
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
    bool isfind = idindexs_.isfind(factory->get_packetid());
    uint16_t index = 
      isfind ? idindexs_.get(factory->get_packetid()) : factorycount_;
    if (factories_[index] != NULL) {
      Assert(false);
      return;
    }
    if (!isfind) idindexs_.add(factory->get_packetid(), index);
    ++factorycount_;
    factories_[index] = factory;
  __LEAVE_FUNCTION
}

void FactoryManager::addfactories_for_serverserver() {
  __ENTER_FUNCTION
    using namespace ps_common_net::packets;
    addfactory(new serverserver::ConnectFactory());
  __LEAVE_FUNCTION
}

bool FactoryManager::isvalid_packetid(uint16_t id) const {
  bool result = false;
  __ENTER_FUNCTION
    result = extend_for_packetid_isvalid();
    return result;
  __LEAVE_FUNCTION
    return result;
}

} //namespace packet

} //namespace ps_common_net
