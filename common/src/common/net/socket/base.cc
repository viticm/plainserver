#include "common/net/socket/base.h"

namespace ps_common_net {

namespace socket {

Base::Base(int32_t socketid) {
  __ENTER_FUNCTION
    init();
    socketid_ = socketid;
    startup();
  __LEAVE_FUNCTION
}

Base::Base(int32_t family, int32_t type, int32_t protocol) {
  __ENTER_FUNCTION
    init();
    open(family, type, protocol);
    startup();
  __LEAVE_FUNCTION
}

Base::~Base() {
  __ENTER_FUNCTION
    close();
  __LEAVE_FUNCTION
}

void Base::init() {
  __ENTER_FUNCTION
    socketid_ = SOCKET_ERROR;
  __LEAVE_FUNCTION
}

void Base::startup() {
  __ENTER_FUNCTION
    uint64_t noblock = 1;
    uint64_t revalue = 1;
    ioctlex(FIONBIO, &noblock);
    struct linger so_linger;
    so_linger.l_onoff = false;
    so_linger.l_linger = 0;
    setoption(SOL_SOCKET, 
              SO_LINGER, 
              reinterpret_cast<char*>(&so_linger), 
              sizeof(so_linger));
    setoption(SOL_SOCKET, 
              SO_REUSEADDR, 
              reinterpret_cast<char*>(&revalue), 
              sizeof(revalue)); 
  __LEAVE_FUNCTION
}

int32_t Base::open(int32_t family, int32_t type, int32_t protocol) {
  __ENTER_FUNCTION
    close();
    socketid_ = api::socketex(family, type, protocol);
    if (socketid_ < 0) return SOCKET_ERROR;
  __LEAVE_FUNCTION
    return SOCKET_ERROR;
}

bool Base::close() {
  __ENTER_FUNCTION
    bool result = true;
    if (socketid_ < 0) return true;
    result = ps_common_file::api::closeex(socketid_);
    socketid_ = SOCKET_ERROR;
    return result;
  __LEAVE_FUNCTION
    return SOCKET_ERROR;
}

int32_t Base::handle() const {
  return socketid_;
}

int32_t Base::send(const void* buffer, int64_t size, int32_t mode) {
  __ENTER_FUNCTION
    int32_t result = api:send(socketid_, buffer, size, mode);
    return result;
  __LEAVE_FUNCTION
    return SOCKET_ERROR;
}

int32_t Base::receive(void* buffer, int64_t size, int32_t mode) {
  __ENTER_FUNCTION
    int32_t result = api:recvex(socketid_, buffer, size, mode);
    return result;
  __LEAVE_FUNCTION
    return SOCKET_ERROR;
}

bool Base::connect(const sockaddr* socket_address) {
  __ENTER_FUNCTION
    socklen_t length = sizeof(struct sockaddr);
    bool connected = api::connectex(socketid_, socket_address, length);
    int32_t error_code = api::getlast_errorcode();
    if (!connectd_ && SOCKET_CONNECT_ERROR == error_code) {
      pollfd fds[1];
      fds[0].fd = socketid_;
      fds[0].events = POLLOUT;
      result = poll(fds, 1, SOCKET_CONNECT_TIMEOUT * 1000);
      if (result > 0) {
        int32_t value, _length = sizeof(value);
        if (!getoption(SOL_SOCKET, 
                       SO_ERROR, 
                       reinterpret_cast<char*>(&value), 
                       &_length) && value) {
          errno = value;
          return false;
        }
        return true;
      }
    }
    return connected;
  __LEAVE_FUNCTION
    return false;
}

bool Base::shutdown(int32_t mode) {
  __ENTER_FUNCTION
    bool result = api::shutdown_ex(socketid_, mode);
    return result;
  __LEAVE_FUNCTION
    return false;
}

bool Base::bind(const sockaddr* socket_address) {
  __ENTER_FUNCTION
    bool result = true;
    socklen_t length = sizeof(struct sockaddr);
    result = api::bindex(socketid_, socket_address, length);
    return result;
  __LEAVE_FUNCTION
    return false;
}

bool Base::listen(int32_t count) {
  __ENTER_FUNCTION
    bool result = true;
    result = api::listenex(socketid_, count);
    return result;
  __LEAVE_FUNCTION
    return false;
}

int32_t Base::accept(sockaddr* socket_address) {
  __ENTER_FUNCTION
    int32_t result = 0;
    socklen_t length = sizeof(struct sockaddr);
    result = api::acceptex(socketid_, socket_address, &length);
    return result;
  __LEAVE_FUNCTION
    return SOCKET_ERROR;
}

bool Base::ioctl(int64_t cmd, uint64_t* argp) {
  __ENTER_FUNCTION
    int32_t result = true;
    result = api::ioctlex(socketid_, cmd, argp);
    return result;
  __LEAVE_FUNCTION
    return false;
}

bool Base::setoption(int32_t level, 
                     int32_t name, 
                     const char* value, 
                     int32_t length) {
  __ENTER_FUNCTION
    bool result = true;
    result = api::setsockopt_ex(socketid_, level, name, value, length);
    return result;
  __LEAVE_FUNCTION
    return false;
}

uint32_t Base::getoption(int32_t level,
                         int32_t name,
                         char* value,
                         uint32_t* length) {
  __ENTER_FUNCTION
    uint32_t result = 0;
    uint32_t option_length = length ? *length : 0;
    uint32_t result = 
      api::getsockopt_exu(socketid_, level, name, value, &option_length);
    return result;
  __LEAVE_FUNCTION
    return 0;
}

int32_t Base::get_errorcode() {
  __ENTER_FUNCTION
    int32_t errorcode = 0;
    errorcode = api::getlast_errorcode();
    return errorcode;
  __LEAVE_FUNCTION
    return SOCKET_ERROR;
}

void Base::get_errorstring(char* buffer, uint16_t length) {
  __ENTER_FUNCTION
    api::getlast_errormessage(buffer, length);
  __LEAVE_FUNCTION
}

} //namespace socket

} //namespace ps_common_net
