// Copyright 2005-2009 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.


#ifndef RE2_UTIL_RANDOM_H__
#define RE2_UTIL_RANDOM_H__

#include "util/util.h"

namespace re2 {

class ACMRandom {
 public:
  ACMRandom(int32 seed) : seed_(seed) {}
  int32 Next();
  int32 Uniform(int32);

  void Reset(int32 seed) { seed_ = seed; }

 private:
  int32 seed_;
};

}  

#endif  
