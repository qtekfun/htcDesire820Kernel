// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_URL_FILE_H_
#define URL_URL_FILE_H_


#include "url/url_parse_internal.h"

namespace url_parse {

#ifdef WIN32

inline bool IsWindowsDriveSeparator(base::char16 ch) {
  return ch == ':' || ch == '|';
}
inline bool IsWindowsDriveLetter(base::char16 ch) {
  return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

#endif  

template<typename CHAR>
inline int FindNextSlash(const CHAR* spec, int begin_index, int spec_len) {
  int idx = begin_index;
  while (idx < spec_len && !IsURLSlash(spec[idx]))
    idx++;
  return idx;
}

#ifdef WIN32

template<typename CHAR>
inline bool DoesBeginWindowsDriveSpec(const CHAR* spec, int start_offset,
                                      int spec_len) {
  int remaining_len = spec_len - start_offset;
  if (remaining_len < 2)
    return false;  
  if (!IsWindowsDriveLetter(spec[start_offset]))
    return false;  
  if (!IsWindowsDriveSeparator(spec[start_offset + 1]))
    return false;  
  return true;
}

template<typename CHAR>
inline bool DoesBeginUNCPath(const CHAR* text,
                             int start_offset,
                             int len,
                             bool strict_slashes) {
  int remaining_len = len - start_offset;
  if (remaining_len < 2)
    return false;

  if (strict_slashes)
    return text[start_offset] == '\\' && text[start_offset + 1] == '\\';
  return IsURLSlash(text[start_offset]) && IsURLSlash(text[start_offset + 1]);
}

#endif  

}  

#endif  
