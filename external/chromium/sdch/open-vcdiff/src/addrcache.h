// Copyright 2007 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_ADDRCACHE_H_
#define OPEN_VCDIFF_ADDRCACHE_H_

#include <config.h>
#include <vector>
#include "vcdiff_defs.h"  

namespace open_vcdiff {

class VCDiffAddressCache {
 public:
  
  static const int kDefaultNearCacheSize = 4;
  static const int kDefaultSameCacheSize = 3;

  VCDiffAddressCache(int near_cache_size, int same_cache_size);

  
  
  VCDiffAddressCache();

  
  
  
  
  
  
  
  bool Init();

  int near_cache_size() const { return near_cache_size_; }

  int same_cache_size() const { return same_cache_size_; }

  
  
  
  
  
  static unsigned char FirstNearMode() {
    return VCD_FIRST_NEAR_MODE;
  }

  
  
  
  
  
  
  unsigned char FirstSameMode() const {
    return VCD_FIRST_NEAR_MODE + near_cache_size();
  }

  
  
  
  unsigned char LastMode() const {
    return FirstSameMode() + same_cache_size() - 1;
  }

  static unsigned char DefaultLastMode() {
    return VCD_FIRST_NEAR_MODE
        + kDefaultNearCacheSize + kDefaultSameCacheSize - 1;
  }

  
  
  
  static bool IsSelfMode(unsigned char mode) {
    return mode == VCD_SELF_MODE;
  }

  static bool IsHereMode(unsigned char mode) {
    return mode == VCD_HERE_MODE;
  }

  bool IsNearMode(unsigned char mode) const {
    return (mode >= FirstNearMode()) && (mode < FirstSameMode());
  }

  bool IsSameMode(unsigned char mode) const {
    return (mode >= FirstSameMode()) && (mode <= LastMode());
  }

  static VCDAddress DecodeSelfAddress(int32_t encoded_address) {
    return encoded_address;
  }

  static VCDAddress DecodeHereAddress(int32_t encoded_address,
                                      VCDAddress here_address) {
    return here_address - encoded_address;
  }

  VCDAddress DecodeNearAddress(unsigned char mode,
                               int32_t encoded_address) const {
    return NearAddress(mode - FirstNearMode()) + encoded_address;
  }

  VCDAddress DecodeSameAddress(unsigned char mode,
                               unsigned char encoded_address) const {
    return SameAddress(((mode - FirstSameMode()) * 256) + encoded_address);
  }

  
  // should be written to the delta file as a variable-length integer;
  // returns false if the encoded address should be written
  
  bool WriteAddressAsVarintForMode(unsigned char mode) const {
    return !IsSameMode(mode);
  }

  
  
  
  
  
  VCDAddress NearAddress(int pos) const {
    return near_addresses_[pos];
  }

  
  
  
  
  
  VCDAddress SameAddress(int pos) const {
    return same_addresses_[pos];
  }

  
  
  
  
  void UpdateCache(VCDAddress address);

  
  
  
  
  
  
  
  
  // should be written to the delta file as a variable-length integer
  
  
  unsigned char EncodeAddress(VCDAddress address,
                              VCDAddress here_address,
                              VCDAddress* encoded_addr);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  VCDAddress DecodeAddress(VCDAddress here_address,
                           unsigned char mode,
                           const char** address_stream,
                           const char* address_stream_end);

 private:
  
  const int near_cache_size_;
  
  const int same_cache_size_;
  // The next position in the NEAR cache to which an address will be written.
  int       next_slot_;
  
  std::vector<VCDAddress> near_addresses_;
  
  std::vector<VCDAddress> same_addresses_;

  
  VCDiffAddressCache(const VCDiffAddressCache&);  
  void operator=(const VCDiffAddressCache&);
};

}  

#endif  
