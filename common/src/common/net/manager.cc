#include "common/base/log.h"
#include "common/base/time_manager.h"
#include "common/base/util.h"
#include "common/net/packet/factorymanager.h"
#include "common/performance/eyes.h"
#include "common/net/manager.h"

#if __WINDOWS__
#pragma warning(disable : 4127) //why use it? for FD_* functions
#endif

namespace ps_common_net {

Manager::Manager() {
  __ENTER_FUNCTION
    FD_ZERO(&readfds_[kSelectFull]);
    FD_ZERO(&writefds_[kSelectFull]);
    FD_ZERO(&exceptfds_[kSelectFull]);
    maxfd_ = minfd_ = SOCKET_INVALID;
    fdsize_ = 0;
    setactive(true);
    onestep_accept_ = NET_ONESTEP_ACCEPT_DEFAULT;
    send_bytes_ = 0;
    receive_bytes_ = 0;
    listenport_ = 0;
  __LEAVE_FUNCTION
}

Manager::~Manager() {
  __ENTER_FUNCTION
    SAFE_DELETE(serversocket_);
  __LEAVE_FUNCTION
}

bool Manager::init(uint16_t connectionmax,
                   uint16_t listenport,
                   const char *listenip) {
  __ENTER_FUNCTION
    /* init packet factory manager { */
    if (!NET_PACKET_FACTORYMANAGER_POINTER)
      g_packetfactory_manager = new packet::FactoryManager();
    Assert(NET_PACKET_FACTORYMANAGER_POINTER);  
    /* } init packet factory manager */
    if (!NET_PACKET_FACTORYMANAGER_POINTER->init()) return false;
    /* server main socket { */
    listenport_ = listenport;
    serversocket_ = new socket::Server(listenport_, listenip);
    Assert(serversocket_);
    listenport_ = 0 == listenport_ ? serversocket_->getport() : listenport_;
    serversocket_->set_nonblocking();
    socketid_ = serversocket_->getid();
    Assert(socketid_ != SOCKET_INVALID);
    FD_SET(socketid_, &readfds_[kSelectFull]);
    FD_SET(socketid_, &exceptfds_[kSelectFull]);
    minfd_ = maxfd_ = socketid_;
    timeout_[kSelectFull].tv_sec = 0;
    timeout_[kSelectFull].tv_usec = 0;
    threadid_ = ps_common_sys::get_current_thread_id();
    uint16_t i;
    for (i = 0; i < NET_OVER_SERVER_MAX; ++i) {
      serverhash_[i] = ID_INVALID;
    }
    /* } server main socket */
    /* connection init { */
    connectionmax_ = connectionmax;
    if (!connectionpool_.init(connectionmax_)) return false;
    connection::Manager::init(connectionmax_);
    /* } connection init */
    return true;
  __LEAVE_FUNCTION
    return false;
}

uint16_t Manager::get_listenport() const {
  return listenport_;
}

uint16_t Manager::get_connectionmax() const {
  return connectionmax_;
}

bool Manager::select() {
  __ENTER_FUNCTION
    timeout_[kSelectUse].tv_sec = timeout_[kSelectFull].tv_sec;
    timeout_[kSelectUse].tv_usec = timeout_[kSelectFull].tv_usec;
    readfds_[kSelectUse] = readfds_[kSelectFull];
    writefds_[kSelectUse] = writefds_[kSelectFull];
    exceptfds_[kSelectUse] = exceptfds_[kSelectFull];
    ps_common_base::util::sleep(100);
    int32_t result = SOCKET_ERROR;
    try {
      result = socket::Base::select(
          maxfd_ + 1,
          &readfds_[kSelectUse],
          &writefds_[kSelectUse],
          &exceptfds_[kSelectUse],
          &timeout_[kSelectUse]);
      Assert(result != SOCKET_ERROR);
    }
    catch(...) {
      FAST_ERRORLOG(kNetLogFile, 
                    "[net] (Manager::select) have error, result: %d", 
                    result);
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Manager::processinput() {
  __ENTER_FUNCTION
    if (SOCKET_INVALID == minfd_ && SOCKET_INVALID == maxfd_)
      return true; //no connection
    uint16_t i;
    //接受新连接的时候至少尝试两次，所以连接池里会多创建一个
    if (FD_ISSET(socketid_, &readfds_[kSelectUse])) {
      for (i = 0; i < onestep_accept_; ++i) {
        if (!accept_newconnection()) break;
      }
    }
    uint16_t connectioncount = connection::Manager::getcount();
    for (i = 0; i < connectioncount; ++i) {
      if (ID_INVALID == connection_idset_[i]) continue;
      connection::Base *connection = NULL;
      connection = connectionpool_.get(connection_idset_[i]);
      Assert(connection);
      int32_t socketid = connection->getsocket()->getid();
      if (socketid_ == socketid) continue;
      if (FD_ISSET(socketid, &readfds_[kSelectUse])) { //read information
        if (connection->getsocket()->iserror()) {
          removeconnection(connection);
        } else {
          try {
            if (!connection->processinput()) { 
              removeconnection(connection);
            } else {
              receive_bytes_ += connection->get_receive_bytes();
            }
          } catch(...) {
            removeconnection(connection);
          }
        }//connection->getsocket()->iserror()
      }
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Manager::processoutput() {
  __ENTER_FUNCTION
    if (SOCKET_INVALID == maxfd_&& SOCKET_INVALID == minfd_)
      return false;
    uint16_t i;
    uint16_t connectioncount = connection::Manager::getcount();
    for (i = 0; i < connectioncount; ++i) {
      if (ID_INVALID == connection_idset_[i]) continue;
      connection::Base* connection = NULL;
      connection = connectionpool_.get(connection_idset_[i]);
      Assert(connection);
      int32_t socketid = connection->getsocket()->getid();
      if (socketid_ == socketid) continue;
      if (FD_ISSET(socketid, &writefds_[kSelectUse])) {
        if (connection->getsocket()->iserror()) {
          removeconnection(connection);
        } else {
          try {
            if (!connection->processoutput()) { 
              removeconnection(connection);
            } else {
              send_bytes_ += connection->get_send_bytes();
            }
          } catch(...) {
            removeconnection(connection);
          }
        } //connection->getsocket()->iserror()
      }
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Manager::processexception() {
  __ENTER_FUNCTION
    if (SOCKET_INVALID == minfd_ && SOCKET_INVALID == maxfd_)
      return true;
    uint16_t connectioncount = connection::Manager::getcount();
    connection::Base* connection = NULL;
    uint16_t i;
    for (i = 0; i < connectioncount; ++i) {
      if (ID_INVALID == connection_idset_[i]) continue;
      connection = connectionpool_.get(connection_idset_[i]);
      Assert(connection);
      int32_t socketid = connection->getsocket()->getid();
      if (socketid_ == socketid) {
        Assert(false);
        continue;
      }
      if (FD_ISSET(socketid, &exceptfds_[kSelectUse])) {
        removeconnection(connection);
      }
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Manager::processcommand() {
  __ENTER_FUNCTION
    if (SOCKET_INVALID == maxfd_&& SOCKET_INVALID == minfd_)
      return false;
    uint16_t i;
    uint16_t connectioncount = connection::Manager::getcount();
    for (i = 0; i < connectioncount; ++i) {
      if (ID_INVALID == connection_idset_[i]) continue;
      connection::Base* connection = NULL;
      connection = connectionpool_.get(connection_idset_[i]);
      Assert(connection);
      int32_t socketid = connection->getsocket()->getid();
      if (socketid_ == socketid) continue;
      if (connection->getsocket()->iserror()) {
        removeconnection(connection);
      } else { //connection is ok
        try {
          if (!connection->processcommand(false)) 
            removeconnection(connection);
        } catch(...) {
          removeconnection(connection);
        }
      } //connection->getsocket()->iserror()
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Manager::accept_newconnection() {
  __ENTER_FUNCTION
    uint32_t step = 0;
    bool result = false;
    connection::Base* newconnection = NULL;
    newconnection = connectionpool_.create();
    if (NULL == newconnection) return false;
    step = 5;
    newconnection->cleanup();
    int32_t socketid = SOCKET_INVALID;
    step = 10;
    try {
      //accept client socket
      result = serversocket_->accept(newconnection->getsocket());
      if (!result) {
        step = 15;
        goto EXCEPTION;
      }
    } catch(...) {
      step += 1000;
      goto EXCEPTION;
    }

    try {
      step = 30;
      socketid = newconnection->getsocket()->getid();
      if (SOCKET_INVALID == socketid) {
        Assert(false);
        goto EXCEPTION;
      }
      step = 40;
      result = newconnection->getsocket()->set_nonblocking();
      if (!result) {
        Assert(false);
        goto EXCEPTION;
      }
      step = 50;
      if (newconnection->getsocket()->iserror()) {
        Assert(false);
        goto EXCEPTION;
      }
      step = 60;
      result = newconnection->getsocket()->setlinger(0);
      if (!result) {
        Assert(false);
        goto EXCEPTION;
      }
      step = 70;
      newconnection->init();
      step = 80;
      try {
        result = addconnection(newconnection);
        if (!result) {
          Assert(false);
          goto EXCEPTION;
        }
      }
      catch(...) {
        step += 10000;
        goto EXCEPTION;
      }
    }
    catch(...) {
      step += 100000;
    }
    FAST_LOG(kNetLogFile,
             "[net] (Manager::accept_newconnection) socketid: %d ",
             newconnection->getsocket()->getid());
    return true;
EXCEPTION:
    newconnection->cleanup();
    connectionpool_.remove(newconnection->getid());
    return false;
  __LEAVE_FUNCTION
    return false;
}

bool Manager::heartbeat() {
  __ENTER_FUNCTION
    uint32_t currenttime = g_time_manager->get_current_time();
    uint16_t connectioncount = connection::Manager::getcount();
    uint16_t i;
    for (i = 0; i < connectioncount; ++i) {
      if (ID_INVALID == connection_idset_[i]) continue;
      connection::Base* connection = NULL;
      connection = connectionpool_.get(connection_idset_[i]);
      if (NULL == connection) {
        Assert(false);
        return false;
      }
      if (!connection->heartbeat(currenttime)) {
        removeconnection(connection);
        Assert(false);
      }
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

void Manager::loop() {
  __ENTER_FUNCTION
    while (isactive()) {
      bool result = false;
      try {
        result = select();
        Assert(result);
        result = processexception();
        Assert(result);
        result = processinput();
        Assert(result);
        result = processoutput();
        Assert(result); 
        if (PERFORMANCE_EYES_POINTER) { //网络性能监视
          uint16_t connectioncount = connection::Manager::getcount();
          PERFORMANCE_EYES_POINTER->set_onlinecount(connectioncount);
          PERFORMANCE_EYES_POINTER->set_connectioncount(connectioncount);
          uint64_t sendbytes = get_send_bytes();
          uint64_t receivebytes = get_receive_bytes();
          PERFORMANCE_EYES_POINTER->set_sendbytes(sendbytes);
          PERFORMANCE_EYES_POINTER->set_receivebytes(receivebytes);
        }
      }
      catch(...) {
        
      }
      try {
        result = processcommand();
        Assert(result);
        //ERRORPRINTF("processcommand");
      }
      catch(...) {
        
      }

      try {
        result = heartbeat();
        Assert(result);
      }
      catch(...) {
      }
    }
  __LEAVE_FUNCTION
}

bool Manager::isactive() {
  return active_;
}

void Manager::setactive(bool active) {
  active_ = active;
}

bool Manager::addconnection(connection::Base* connection) {
  __ENTER_FUNCTION
    if (fdsize_ > FD_SETSIZE) {
      Assert(false);
      return false;
    }
    if (!connection::Manager::add(connection)) {
      Assert(false);
      return false;
    }
    int32_t socketid = connection->getsocket()->getid();
    Assert(SOCKET_INVALID != socketid);
    minfd_ = min(socketid, minfd_);
    maxfd_ = max(socketid, maxfd_);
    FD_SET(socketid, &readfds_[kSelectFull]);
    FD_SET(socketid, &writefds_[kSelectFull]);
    FD_SET(socketid, &exceptfds_[kSelectFull]);
    ++fdsize_;
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Manager::deleteconnection(connection::Base* connection) {
  __ENTER_FUNCTION
    int32_t socketid = connection->getsocket()->getid();
    uint16_t i;
    Assert(minfd_ != SOCKET_INVALID && maxfd_ != SOCKET_INVALID);   
    if (socketid == minfd_) { //the first connection
      int32_t socketid_max = maxfd_;
      uint16_t connectioncount = connection::Manager::getcount();
      for (i = 0; i < connectioncount; ++i) {
        if (ID_INVALID == connection_idset_[i]) continue;
        connection = connectionpool_.get(connection_idset_[i]);
        Assert(connection);
        if (NULL == connection) continue;
        int32_t _socketid = connection->getsocket()->getid();
        if (socketid == _socketid || SOCKET_INVALID == _socketid) continue;
        if (socketid_max < _socketid) socketid_max = _socketid;
        if (minfd_ == maxfd_) {
          minfd_ = maxfd_ = SOCKET_INVALID;
        } else {
          minfd_ = socketid_max > socketid_ ? minfd_ : socketid_max;
        }
      }
    } else if (socketid == maxfd_) { //
      int32_t socketid_min = minfd_;
      uint16_t connectioncount = connection::Manager::getcount();
      for (i = 0; i < connectioncount; ++i) {
        if (ID_INVALID == connection_idset_[i]) continue;
        connection = connectionpool_.get(connection_idset_[i]);
        Assert(connection);
        if (NULL == connection) continue;
        int32_t _socketid = connection->getsocket()->getid();
        if (socketid == _socketid || SOCKET_INVALID == _socketid) continue;
        if (socketid_min > _socketid) socketid_min = _socketid;
        if (minfd_ == maxfd_) {
          minfd_ = maxfd_ = SOCKET_INVALID;
        } else {
          maxfd_ = socketid_min < socketid_ ? maxfd_ : socketid_min;
        }
      }
    }
    FD_CLR(static_cast<uint32_t>(socketid), &readfds_[kSelectFull]);
    FD_CLR(static_cast<uint32_t>(socketid), &readfds_[kSelectUse]);
    FD_CLR(static_cast<uint32_t>(socketid), &writefds_[kSelectFull]);
    FD_CLR(static_cast<uint32_t>(socketid), &writefds_[kSelectUse]);
    FD_CLR(static_cast<uint32_t>(socketid), &exceptfds_[kSelectFull]);
    FD_CLR(static_cast<uint32_t>(socketid), &exceptfds_[kSelectUse]);
    --fdsize_;
    Assert(fdsize_ >= 0);
    removeconnection(connection->getid());
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Manager::removeconnection(connection::Base* connection) {
  __ENTER_FUNCTION
    //first clean in connection manager
    Assert(deleteconnection(connection));
    Assert(connection != NULL);
    //服务器的连接，都不主动关闭
    FAST_LOG(kNetLogFile, 
             "[net] (Manager::removeconnection) id: %d", 
             connection->getid());
    return true;
  __LEAVE_FUNCTION
    return false;
}

void Manager::removeconnection(int16_t id) {
  __ENTER_FUNCTION
    Assert(count_ > 0);
    connection::Base* connection = NULL;
    connection = connectionpool_.get(id);
    if (NULL == connection) {
      Assert(false);
      return;
    }
    int16_t managerid = connection->get_managerid();
    if (managerid >= static_cast<int16_t>(sizeof(connection_idset_))) {
      Assert(false);
      return;
    }
    connection = connectionpool_.get(connection_idset_[count_ - 1]);
    if (NULL == connection) {
      Assert(false);
      return;
    }
    connection_idset_[managerid] = connection_idset_[count_ - 1];
    connection_idset_[count_ - 1] = ID_INVALID;
    connection->set_managerid(managerid);
    --count_;
    Assert(count_ >= 0);
  __LEAVE_FUNCTION
}

void Manager::remove_allconnection() {
  __ENTER_FUNCTION
    uint16_t connectioncount = connection::Manager::getcount();
    uint16_t i;
    for (i = 0; i < connectioncount; ++i) {
      if (ID_INVALID == connection_idset_[0]) break;
      connection::Base* connection = NULL;
      connection = connectionpool_.get(connection_idset_[i]);
      if (NULL == connection) {
        Assert(false);
        break;
      }
      removeconnection(connection);
    }
  __LEAVE_FUNCTION
}

connection::Base *Manager::getconnection(uint16_t id) {
  __ENTER_FUNCTION 
    Assert(id >= 0 && id < NET_OVER_SERVER_MAX);
    int16_t connectionid = serverhash_[id];
    connection::Base *connection = NULL;
    connection = connectionpool_.get(connectionid);
    Assert(connection);
    return connection;
  __LEAVE_FUNCTION
    return NULL;
}

void Manager::broadcast(packet::Base* packet) {
  __ENTER_FUNCTION
    uint16_t connectioncount = connection::Manager::getcount();
    uint16_t i;
    for (i = 0; i < connectioncount; ++i) {
      if (ID_INVALID == connection_idset_[i]) continue;
      connection::Base* connection = NULL;
      connection = connectionpool_.get(connection_idset_[i]);
      if (NULL == connection) {
        Assert(false); 
        continue;
      }
      connection->sendpacket(packet);
    }
  __LEAVE_FUNCTION
}

int32_t Manager::get_onestep_accept() const {
  return onestep_accept_;
}

void Manager::set_onestep_accept(int32_t count) {
  onestep_accept_ = count;
}

uint64_t Manager::get_send_bytes() const {
  return send_bytes_;
}
   
uint64_t Manager::get_receive_bytes() const {
  return receive_bytes_;
}

connection::Pool *Manager::get_connectionpool() {
  return &connectionpool_;
}

} //namespace ps_common_net
