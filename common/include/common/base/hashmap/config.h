/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id config.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/27 11:29
 * @uses the base hashmap module config
 */
#ifndef PS_COMMON_BASE_HASHMAP_INTEGER_H_
#define PS_COMMON_BASE_HASHMAP_INTEGER_H_

#include "common/base/config.h"

#undef max
#undef min

#ifdef __DEPRECATED /* { */
#include <tr1/unordered_map>
#elif __LINUX__ /* }{ */
#include <ext/hash_map>
#elif __WINDOWS__ /* }{ */
#include <hash_map>
#endif /* } */

#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifdef __DEPRECATED /* { */
#define hash_map std::tr1::unordered_map
#elif __SGI_STL_PORT /* }{ */
#define hash_map std::hash_map
#elif __LINUX__ /* }{ */
#define hash_map __gnu_cxx::hash_map
#elif __WINDOWS__ /* }{ */
#define hash_map stdext::hash_map
#endif /* } */

#endif //PS_COMMON_BASE_HASHMAP_INTEGER_H_
