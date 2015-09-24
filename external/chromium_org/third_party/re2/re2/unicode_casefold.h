// Copyright 2008 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.



#ifndef RE2_UNICODE_CASEFOLD_H__
#define RE2_UNICODE_CASEFOLD_H__

#include "util/util.h"

namespace re2 {

enum {
  EvenOdd = 1,
  OddEven = -1,
  EvenOddSkip = 1<<30,
  OddEvenSkip,
};

struct CaseFold {
  uint32 lo;
  uint32 hi;
  int32 delta;
};

extern CaseFold unicode_casefold[];
extern int num_unicode_casefold;

extern CaseFold unicode_tolower[];
extern int num_unicode_tolower;

extern CaseFold* LookupCaseFold(CaseFold*, int, Rune rune);

extern Rune ApplyFold(CaseFold *f, Rune r);

}  

#endif  
