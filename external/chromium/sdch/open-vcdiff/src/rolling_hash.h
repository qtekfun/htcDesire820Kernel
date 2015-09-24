// Copyright 2007, 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_ROLLING_HASH_H_
#define OPEN_VCDIFF_ROLLING_HASH_H_

#include <config.h>
#include <stdint.h>  
#include "logging.h"

namespace open_vcdiff {


class RollingHashUtil {
 public:
  
  
  static const uint32_t kMult = 257;

  
  
  
  static const uint32_t kBase = (1 << 23);

  
  static inline uint32_t ModBase(uint32_t operand) {
    return operand & (kBase - 1);
  }

  
  
  
  
  
  static inline uint32_t FindModBaseInverse(uint32_t operand) {
    
    
    
    
    
    
    
    
    
    
    return ModBase(uint32_t(0) - operand);
  }

  
  
  
  
  static inline uint32_t HashStep(uint32_t partial_hash,
                                  unsigned char next_byte) {
    return ModBase((partial_hash * kMult) + next_byte);
  }

  
  
  
  
  
  
  static inline uint32_t HashFirstTwoBytes(const char* ptr) {
    return (static_cast<unsigned char>(ptr[0]) * kMult)
        + static_cast<unsigned char>(ptr[1]);
  }
 private:
  
  
  RollingHashUtil();
  RollingHashUtil(const RollingHashUtil&);  
  void operator=(const RollingHashUtil&);
};

template<int window_size>
class RollingHash {
 public:
  
  
  
  
  
  
  
  
  
  static bool Init();

  
  
  
  RollingHash() {
    if (!remove_table_) {
      LOG(DFATAL) << "RollingHash object instantiated"
                     " before calling RollingHash::Init()" << LOG_ENDL;
    }
  }

  
  static uint32_t Hash(const char* ptr) {
    uint32_t h = RollingHashUtil::HashFirstTwoBytes(ptr);
    for (int i = 2; i < window_size; ++i) {
      h = RollingHashUtil::HashStep(h, ptr[i]);
    }
    return h;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  uint32_t UpdateHash(uint32_t old_hash,
                      const char old_first_byte,
                      const char new_last_byte) const {
    uint32_t partial_hash = RemoveFirstByteFromHash(old_hash, old_first_byte);
    return RollingHashUtil::HashStep(partial_hash, new_last_byte);
  }

 protected:
  
  
  
  
  
  static uint32_t RemoveFirstByteFromHash(uint32_t full_hash,
                                          unsigned char first_byte) {
    return RollingHashUtil::ModBase(full_hash + remove_table_[first_byte]);
  }

 private:
  
  
  static const uint32_t* remove_table_;
};

template<int window_size>
const uint32_t* RollingHash<window_size>::remove_table_ = NULL;

template<int window_size>
bool RollingHash<window_size>::Init() {
  if (window_size < 2) {
    LOG(ERROR) << "RollingHash window size " << window_size
               << " is too small" << LOG_ENDL;
    return false;
  }
  if (remove_table_ == NULL) {
    
    
    
    
    
    
    
    
    
    
    uint32_t* new_remove_table = new uint32_t[256];
    
    
    
    uint32_t multiplier = 1;
    for (int i = 0; i < window_size - 1; ++i) {
      multiplier =
          RollingHashUtil::ModBase(multiplier * RollingHashUtil::kMult);
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    uint32_t byte_times_multiplier = 0;
    for (int removed_byte = 0; removed_byte < 256; ++removed_byte) {
      new_remove_table[removed_byte] =
          RollingHashUtil::FindModBaseInverse(byte_times_multiplier);
      
      
      byte_times_multiplier =
          RollingHashUtil::ModBase(byte_times_multiplier + multiplier);
    }
    remove_table_ = new_remove_table;
  }
  return true;
}

}  

#endif  
