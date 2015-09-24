// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRING_UTIL_H_
#define BASE_STRING_UTIL_H_

#include <string.h>

namespace base {

#ifdef WIN32
inline int strcasecmp(const char* s1, const char* s2) {
  return _stricmp(s1, s2);
}
#else
inline int strcasecmp(const char* s1, const char* s2) {
  return ::strcasecmp(s1, s2);
}
#endif

}

#endif  
