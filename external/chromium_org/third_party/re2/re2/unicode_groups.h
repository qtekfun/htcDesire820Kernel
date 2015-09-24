// Copyright 2008 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.



#ifndef RE2_UNICODE_GROUPS_H__
#define RE2_UNICODE_GROUPS_H__

#include "util/util.h"

namespace re2 {

struct URange16
{
  uint16 lo;
  uint16 hi;
};

struct URange32
{
  uint32 lo;
  uint32 hi;
};

struct UGroup
{
  const char *name;
  int sign;  
  URange16 *r16;
  int nr16;
  URange32 *r32;
  int nr32;
};

extern UGroup unicode_groups[];
extern int num_unicode_groups;

extern UGroup posix_groups[];
extern int num_posix_groups;

extern UGroup perl_groups[];
extern int num_perl_groups;

}  

#endif  
