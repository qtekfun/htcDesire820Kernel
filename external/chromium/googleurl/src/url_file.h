// Copyright 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLEURL_SRC_URL_FILE_H__
#define GOOGLEURL_SRC_URL_FILE_H__

#include "googleurl/src/url_parse_internal.h"

namespace url_parse {

#ifdef WIN32

inline bool IsWindowsDriveSeparator(char16 ch) {
  return ch == ':' || ch == '|';
}
inline bool IsWindowsDriveLetter(char16 ch) {
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
