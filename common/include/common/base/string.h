/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id string.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.it@gmail.com>
 * @date 2014/06/18 13:45
 * @uses base string module
 */
#ifndef PS_COMMON_BASE_STRING_H_
#define PS_COMMON_BASE_STRING_H_

#include <map> 
#include "common/base/config.h"

namespace ps_common_base {

namespace string {

/*typedef {*/
typedef std::map<std::string, std::string> map; //my map
/*typedef }*/

char get_base64char(int index);


void replace_all(std::string& str, 
                 const std::string source, 
                 const std::string destination);

bool toint16(const char* source, 
             int16_t& result, 
             uint8_t converted_length = 0, 
             bool ignored_zero = false);

bool toint32(const char* source, 
             int32_t& result, 
             uint8_t converted_length = 0, 
             bool ignored_zero = false);

void encrypt(const char* in, char* out, int32_t out_length);

void decrypt(const char* in, char* out, int32_t out_length);

char* safecopy(char* dest, const char* src, size_t size);

}; //namespace string

}; //namespace ps_common_base

#endif //PS_COMMON_BASE_STRING_H_
