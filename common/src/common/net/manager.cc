#include "common/base/log.h"
#include "common/base/time_manager.h"
#include "common/base/util.h"
#include "common/net/packet/factorymanager.h"
#include "common/net/packets/serverserver/connect.h"
#include "common/net/manager.h"

#if __WINDOWS__
#pragma warning(disable : 4127) //why use it? for FD_* functions
#endif

//ps_common_net::Manager* g_netmanager = NULL;

namespace ps_common_net {

/**
template<> Manager* ps_common_base::Singleton<Manager>::singleton_ = NULL;

Manager* Manager::getsingleton_pointer() {
  return singleton_;
}

Manager& Manager::getsingleton() {
  Assert(singleton_);
  return *singleton_;
}
**/

Manager::Manager(uint16_t port) {
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
    listenport_ = port;
    billing_serverconnection_.setid(0);
  __LEAVE_FUNCTION
}

Manager::~Manager() {
  __ENTER_FUNCTION
    SAFE_DELETE(serversocket_);
  __LEAVE_FUNCTION
}

bool Manager::init(uint16_t connectionmax) {
  __ENTER_FUNCTION
    /* init packet factory manager { */
    if (!NET_PACKET_FACTORYMANAGER_POINTER)
      g_packetfactory_manager = new packet::FactoryManager();
    Assert(NET_PACKET_FACTORYMANAGER_POINTER);  
    /* } init packet factory manager */

    /* server main socket { */
    serversocket_ = new socket::Server(listenport_);
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
    if (FD_ISSET(socketid_, &readfds_[kSelectUse])) {
      for (i = 0; i < onestep_accept_; ++i) {
        if (!accept_newconnection()) break;
      }
    }
    uint16_t connectioncount = connection::Manager::getcount();
    for (i = 0; i < connectioncount; ++i) {
      if (ID_INVALID == connection_idset_[i]) continue;
      connection::Server* serverconnection = NULL;
      serverconnection = connectionpool_.get(connection_idset_[i]);
      Assert(serverconnection);
      int32_t socketid = serverconnection->getsocket()->getid();
      if (socketid_ == socketid) continue;
      if (FD_ISSET(socketid, &readfds_[kSelectUse])) { //read information
        if (serverconnection->getsocket()->iserror()) {
          removeconnection(serverconnection);
        }
        else {
          try {
            if (!serverconnection->processinput()) { 
              removeconnection(serverconnection);
            }
            else {
              receive_bytes_ += serverconnection->get_receive_bytes();
            }
          }
          catch(...) {
            removeconnection(serverconnection);
          }
        }
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
      connection::Server* serverconnection = NULL;
      serverconnection = connectionpool_.get(connection_idset_[i]);
      //serverconnection = &billing_serverconnection_;
      Assert(serverconnection);
      int32_t socketid = serverconnection->getsocket()->getid();
      if (socketid_ == socketid) continue;
      if (FD_ISSET(socketid, &writefds_[kSelectUse])) {
        if (serverconnection->getsocket()->iserror()) {
          removeconnection(serverconnection);
        }
        else {
          try {
            if (!serverconnection->processoutput()) { 
              removeconnection(serverconnection);
            }
            else {
              send_bytes_ += serverconnection->get_send_bytes();
            }
          }
          catch(...) {
            removeconnection(serverconnection);
          }
        }
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
    connection::Server* serverconnection = NULL;
    uint16_t i;
    for (i = 0; i < connectioncount; ++i) {
      if (ID_INVALID == connection_idset_[i]) continue;
      serverconnection = connectionpool_.get(connection_idset_[i]);
      Assert(serverconnection);
      int32_t socketid = serverconnection->getsocket()->getid();
      if (socketid_ == socketid) {
        Assert(false);
        continue;
      }
      if (FD_ISSET(socketid, &exceptfds_[kSelectUse])) {
        removeconnection(serverconnection);
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
      connection::Server* serverconnection = NULL;
      serverconnection = connectionpool_.get(connection_idset_[i]);
      //serverconnection = &billing_serverconnection_;
      Assert(serverconnection);
      int32_t socketid = serverconnection->getsocket()->getid();
      if (socketid_ == socketid) continue;
      if (serverconnection->getsocket()->iserror()) {
        removeconnection(serverconnection);
      }
      else { //connection is ok
        try {
          if (!serverconnection->processcommand(false)) 
            removeconnection(serverconnection);
        }
        catch(...) {
          removeconnection(serverconnection);
        }
      }
    }
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Manager::accept_newconnection() {
  __ENTER_FUNCTION
    uint32_t step = 0;
    bool result = false;
    connection::Server* newconnection = NULL;
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
    }
    catch(...) {
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
      newconnection->setstatus(kConnectionStatusCenterConnect);
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
    FAST_ERRORLOG(kNetLogFile,
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
        //ERRORPRINTF("select");
        result = processexception();
        Assert(result);
        //ERRORPRINTF("processexception");
        result = processinput();
        Assert(result);
        //ERRORPRINTF("processinput");
        result = processoutput();
        Assert(result); 
        //ERRORPRINTF("processoutput");
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
    connection::Server* serverconnection = NULL;
    serverconnection = //class pointer transform use dynamic_cast will be safe
      dynamic_cast<connection::Server*>(connection);
    int32_t socketid = serverconnection->getsocket()->getid();
    uint16_t i;
    Assert(minfd_ != SOCKET_INVALID && maxfd_ != SOCKET_INVALID);   
    if (socketid == minfd_) { //the first connection
      int32_t socketid_max = maxfd_;
      uint16_t connectioncount = connection::Manager::getcount();
      for (i = 0; i < connectioncount; ++i) {
        if (ID_INVALID == connection_idset_[i]) continue;
        serverconnection = connectionpool_.get(connection_idset_[i]);
        Assert(serverconnection);
        if (NULL == serverconnection) continue;
        int32_t _socketid = serverconnection->getsocket()->getid();
        if (socketid == _socketid || SOCKET_INVALID == _socketid) continue;
        if (socketid_max < _socketid) socketid_max = _socketid;
        if (minfd_ == maxfd_) {
          minfd_ = maxfd_ = SOCKET_INVALID;
        }
        else {
          if (socketid_max > socketid_) {
            minfd_ = socketid_;
          }
          else {
            minfd_ = socketid_max;
          }
        }
      }
    }
    else if (socketid == maxfd_) { //
      int32_t socketid_min = minfd_;
      uint16_t connectioncount = connection::Manager::getcount();
      for (i = 0; i < connectioncount; ++i) {
        if (ID_INVALID == connection_idset_[i]) continue;
        serverconnection = connectionpool_.get(connection_idset_[i]);
        Assert(serverconnection);
        if (NULL == serverconnection) continue;
        int32_t _socketid = serverconnection->getsocket()->getid();
        if (socketid == _socketid || SOCKET_INVALID == _socketid) continue;
        if (socketid_min > _socketid) socketid_min = _socketid;
        if (minfd_ == maxfd_) {
          minfd_ = maxfd_ = SOCKET_INVALID;
        }
        else {
          if (socketid_min < socketid_) {
            maxfd_ = socketid_;
          }
          else {
            maxfd_ = socketid_min;
          }
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
    connection::Manager::remove(serverconnection->getid());
    return true;
  __LEAVE_FUNCTION
    return false;
}

bool Manager::removeconnection(connection::Base* connection) {
  __ENTER_FUNCTION
    //first clean in connection manager
    Assert(deleteconnection(connection));
    connection::Server* serverconnection = NULL;
    serverconnection = dynamic_cast<connection::Server*>(connection);
    Assert(serverconnection != NULL);
    //second clean in connection pool, free to new connection
    serverconnection->freeown();
    FAST_ERRORLOG(kNetLogFile, 
                  "[net] (Manager::removeconnection) id: %d", 
                  connection->getid());
    return true;
  __LEAVE_FUNCTION
    return false;
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

connection::Server* Manager::get_serverconnection(uint16_t id) {
  __ENTER_FUNCTION 
    Assert(id >= 0 && id < NET_OVER_SERVER_MAX);
    int16_t connectionid = serverhash_[id];
    connection::Server* serverconnection = NULL;
    serverconnection = connectionpool_.get(connectionid);
    Assert(serverconnection);
    return serverconnection;
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

bool Manager::connectserver() {
  uint8_t step = 0;
  __ENTER_FUNCTION
    /**
    bool result = false;
    packets::serverserver::Connect* connectpacket = NULL;
    pap_common_net::socket::Base* billingsocket = NULL;
	  const char *kServerIp = "127.0.0.1";
	  const uint16_t kServerPort = 12680;
    billingsocket = billing_serverconnection_.getsocket();
    try {
      result = billingsocket->create();
      if (!result) {
        step = 1;
        Assert(false);
      }
      result = billingsocket->connect(
          kServerIp,
		  kServerPort);
      if (!result) {
        step = 2;
        printf("exception 2");
        goto EXCEPTION;
        Assert(false);
      }
      result = billingsocket->set_nonblocking();
      if (!result) {
        step = 3;
        printf("exception 3");
        Assert(false);
      }

      result = billingsocket->setlinger(0);
      if (!result) {
        step = 4;
        printf("exception 4");
        Assert(false);
      }
      g_log->fast_save_log(kBillingLogFile,
                           "Manager::connectserver()"
                           " ip:%s, port: %d, success",
						   kServerIp,
						   kServerPort);
    }
    catch(...) {
      step = 5;
      Assert(false);
    }
    result = addconnection(
        (pap_server_common_net::connection::Base*)&billing_serverconnection_);
    if (!result) {
      step = 6;
      Assert(false);
    }
    connectpacket = new pap_server_common_net::packets::serverserver::Connect();
    connectpacket->set_serverid(9999);
    connectpacket->set_worldid(0);
    connectpacket->set_zoneid(0);
    result = billing_serverconnection_.sendpacket(connectpacket);
    SAFE_DELETE(connectpacket);
    if (!result) {
      step = 7;
      Assert(false);
    }
    g_log->fast_save_log(kBillingLogFile, 
                         "Manager::connectserver() is success!");
    return true;
EXCEPTION:
    g_log->fast_save_log(
        kBillingLogFile, 
        "Manager::connectserver() have error, ip: %s, port: %d, step: %d",
		kServerIp,
		kServerPort,
        step);
    billing_serverconnection_.cleanup();
    **/
    return false;
  __LEAVE_FUNCTION
    return false;
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

} //namespace ps_common_net
