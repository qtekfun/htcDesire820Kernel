// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_VARINT_BIGENDIAN_H_
#define OPEN_VCDIFF_VARINT_BIGENDIAN_H_


#include <config.h>
#include <stdint.h>  
#include <string>
#include "vcdiff_defs.h"  

namespace open_vcdiff {

class OutputStringInterface;

template <typename SignedIntegerType> class VarintMaxBytes;

template<> class VarintMaxBytes<int32_t> {
 public:
  static const int kMaxBytes = 5;
};

template<> class VarintMaxBytes<int64_t> {
 public:
  static const int kMaxBytes = 9;
};

template <typename SignedIntegerType>
class VarintBE {  
 public:
  typedef std::string string;

  
  static const SignedIntegerType kMaxVal;

  
  
  static const int kMaxBytes = VarintMaxBytes<SignedIntegerType>::kMaxBytes;

  
  
  
  
  
  
  
  
  
  
  static SignedIntegerType Parse(const char* limit, const char** ptr);

  
  static int Length(SignedIntegerType v);

  
  
  
  static int Encode(SignedIntegerType v, char* ptr);

  
  
  static void AppendToString(SignedIntegerType value, string* s);

  
  
  static void AppendToOutputString(SignedIntegerType value,
                                   OutputStringInterface* output_string);

 private:
  
  
  
  
  
  static int EncodeInternal(SignedIntegerType v, char* varint_buf);

  
  VarintBE();
  VarintBE(const VarintBE&);  
  void operator=(const VarintBE&);
};

}  

#endif  
