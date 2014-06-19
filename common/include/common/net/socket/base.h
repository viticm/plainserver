/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id base.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/19 14:51
 * @uses socket base class
 */
#ifndef PS_COMMON_NET_SOCKET_BASE_H_
#define PS_COMMON_NET_SOCKET_BASE_H_

#include "common/file/api.h"
#include "common/net/socket/api.h"

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

#if __WINDOWS__
#ifndef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOC
#endif
#endif

#define SOCKET_WOULD_BLOCK EWOULDBLOCK //api use SOCKET_ERROR_WOULD_BLOCK
#define SOCKET_CONNECT_ERROR EINPROGRESS
#define SOCKET_CONNECT_TIMEOUT 10

namespace pap_common_net {

namespace socket {

class Base {

 public:
   Base(int32_t socketid);
   Base(int32_t family, int32_t type, int32_t protocol);
   virtual ~Base();

 public:
   void startup();
   int32_t send(const void* buffer, int64_t size, int32_t mode);
   int32_t receive(void* buffer, int64_t size, int32_t mode);
   bool ioctl(int64_t cmd, uint64_t* argp);
   bool setoption(int32_t level, 
                  int32_t name, 
                  const char* value, 
                  int32_t length);
   uint32_t getoption(int32_t level,
                      int32_t name,
                      char* value,
                      uint32_t* length);
   int32_t handle() const;

 public:
   int32_t open(int32_t family, int32_t type, int32_t protocol);
   bool close();
   bool connect(const sockaddr* socket_address);
   bool shutdown(int32_t mode);
   bool bind(const sockaddr* socket_address);
   bool listen(int32_t count);
   int32_t accept(sockaddr* socket_address);
   int32_t get_errorcode();
   void get_errorstring(char* buffer, uint16_t length);

 protected:
   int32_t socketid_;
   void init();

};

}; //namespace socket

}; //namespace ps_common_net

#include "common/net/socket/extend.inl"

#endif //PS_COMMON_NET_SOCKET_BASE_H_
