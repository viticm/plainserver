/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id util.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/18 13:46
 * @uses base util module
 */
#ifndef PS_COMMON_BASE_UTIL_H_
#define PS_COMMON_BASE_UTIL_H_

#include "common/base/md5.h"
#include "common/base/config.h"

namespace ps_common_base {

namespace util {

char value_toascii(char in);
char ascii_tovalue(char in);
bool binary_tostring(const char* in, uint32_t in_length, char* out);
bool string_tobinary(const char* in, 
                     uint32_t in_length, 
                     char* out, 
                     uint32_t out_limit, 
                     uint32_t &out_length);
void sleep(uint32_t million_seconds);
uint32_t str_length(const char* str);
void char_swap(char* str, uint32_t source, uint32_t destination);
void simple_encrypt_decrypt(char* str, 
                            uint32_t strlength, 
                            uint32_t key_begin = 0); //use first is encrypt 
                                                     //and next to decrypt
int charset_convert(const char* from, 
                    const char* to, 
                    char* save, 
                    int savelen, 
                    const char* src, 
                    int srclen) ;
void password_swap_chars(char* str); //string will more than 32

} //namespace util

} //namespace ps_common_base

#endif //PS_COMMON_BASE_UTIL_H_
