// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_CHECKSUM_H_
#define OPEN_VCDIFF_CHECKSUM_H_

#include <config.h>
#include "zlib.h"

namespace open_vcdiff {

typedef uLong VCDChecksum;

const VCDChecksum kNoPartialChecksum = 0;

inline VCDChecksum ComputeAdler32(const char* buffer,
                                  size_t size) {
  return adler32(kNoPartialChecksum,
                 reinterpret_cast<const Bytef*>(buffer),
                 static_cast<uInt>(size));
}

inline VCDChecksum UpdateAdler32(VCDChecksum partial_checksum,
                                 const char* buffer,
                                 size_t size) {
  return adler32(partial_checksum,
                 reinterpret_cast<const Bytef*>(buffer),
                 static_cast<uInt>(size));
}

}  

#endif  
