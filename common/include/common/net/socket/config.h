/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id config.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/19 14:52
 * @uses socket module base config
 */
#ifndef PS_COMMON_NET_SOCKET_CONFIG_H_
#define PS_COMMON_NET_SOCKET_CONFIG_H_

#include "common/net/config.h"

#define SOCKETINPUT_BUFFERSIZE_DEFAULT (64*1024) //default size
#define SOCKETINPUT_DISCONNECT_MAXSIZE (96*1024) //if buffer more than it,
                                                 //will disconnect this socket.
#define SOCKETOUTPUT_BUFFERSIZE_DEFAULT (8192)   //default size
#define SOCKETOUTPUT_DISCONNECT_MAXSIZE (100*1024)//if buffer more than it,
                                                  //will disconnect this socket.

#endif //PS_COMMON_NET_SOCKET_CONFIG_H_
