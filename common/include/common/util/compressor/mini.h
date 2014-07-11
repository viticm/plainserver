/**
 * PLAIN SERVER Engine ( https://github.com/viticm/plainserver )
 * $Id mini.h
 * @link https://github.com/viticm/plianserver for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/07/11 13:42
 * @uses the util mini compressor class
 */
#ifndef PS_COMMON_UTIL_COMPRESSOR_MINI_H_
#define PS_COMMON_UTIL_COMPRESSOR_MINI_H_

#include "common/util/compressor/config.h"

#if __WINDOWS__ /* { */
#undef __LINUX__
#elif __LINUX__ /* }{ */
#undef __WINDOWS__
#endif /* } */

#include "common/util/compressor/minilzo.h"

#ifndef __WINDOWS__
#define __WINDOWS__ (defined(_MSC_VER) || defined(__ICL))
#endif
#ifndef __LINUX__
#define __LINUX__ !(__WINDOWS__)
#endif

#define UTIL_COMPRESSOR_MINI_GET_OUTLENGTH(in) ((in)+(in)/16+64+3)
#define UTIL_COMPRESSOR_MINI_GET_ESCAPE_OUTLENGTH(in) \
  (2*UTIL_COMPRESSOR_MINI_GET_OUTLENGTH(in)+1)
#define UTIL_COMPRESSOR_MINI_SIZE_MAX (1024*1024)

namespace ps_common_util {

namespace compressor {

class Mini {

 public:
   Mini();
   ~Mini();

 public:
   static bool init();
   bool compress(const unsigned char *in,
                 uint32_t insize,
                 unsigned char *out,
                 uint64_t& outsize);
   bool compress_andescape(const unsigned char *in,
                           uint32_t insize,
                           unsigned char *out,
                           uint32_t outsize);
   bool decompress(const unsigned char *in, uint32_t insize);
   const unsigned char *get_decompress_buffer();
   uint32_t get_decompress_buffersize() const;

 private:
   unsigned char work_memory_[LZO1X_1_MEM_COMPRESS];
   unsigned char decompress_buffer_[UTIL_COMPRESSOR_MINI_SIZE_MAX];
   uint64_t decompress_buffersize_;

};

}; //namespace compressor

}; //namespace ps_common_util


#endif //PS_COMMON_UTIL_COMPRESSOR_MINI_H_
