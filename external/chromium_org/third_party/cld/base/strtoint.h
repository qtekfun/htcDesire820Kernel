// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_STRTOINT_H_
#define BASE_STRTOINT_H_

#include <stdlib.h> 
#include <string>
#include "base/port.h"
#include "base/basictypes.h"

int32 strto32_adapter(const char *nptr, char **endptr, int base);
uint32 strtou32_adapter(const char *nptr, char **endptr, int base);

inline int32 strto32(const char *nptr, char **endptr, int base) {
  if (sizeof(int32) == sizeof(long))
    return strtol(nptr, endptr, base);
  else
    return strto32_adapter(nptr, endptr, base);
}

inline uint32 strtou32(const char *nptr, char **endptr, int base) {
  if (sizeof(uint32) == sizeof(unsigned long))
    return strtoul(nptr, endptr, base);
  else
    return strtou32_adapter(nptr, endptr, base);
}

inline int64 strto64(const char *nptr, char **endptr, int base) {
  COMPILE_ASSERT(sizeof(int64) == sizeof(long long),
                 sizeof_int64_is_not_sizeof_long_long);
  return strtoll(nptr, endptr, base);
}

inline uint64 strtou64(const char *nptr, char **endptr, int base) {
  COMPILE_ASSERT(sizeof(uint64) == sizeof(unsigned long long),
                 sizeof_uint64_is_not_sizeof_long_long);
  return strtoull(nptr, endptr, base);
}

inline int32 atoi32(const char *nptr) {
  return strto32(nptr, NULL, 10);
}

inline int64 atoi64(const char *nptr) {
  return strto64(nptr, NULL, 10);
}

inline int32 atoi32(const string &s) {
  return atoi32(s.c_str());
}

inline int64 atoi64(const string &s) {
  return atoi64(s.c_str());
}

#endif  
