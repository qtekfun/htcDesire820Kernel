// Copyright 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLEURL_SRC_URL_PARSE_INTERNAL_H__
#define GOOGLEURL_SRC_URL_PARSE_INTERNAL_H__

#include "googleurl/src/url_parse.h"

namespace url_parse {

inline bool IsURLSlash(char16 ch) {
  return ch == '/' || ch == '\\';
}

inline bool ShouldTrimFromURL(char16 ch) {
  return ch <= ' ';
}

template<typename CHAR>
inline void TrimURL(const CHAR* spec, int* begin, int* len) {
  
  while (*begin < *len && ShouldTrimFromURL(spec[*begin]))
    (*begin)++;

  
  
  while (*len > *begin && ShouldTrimFromURL(spec[*len - 1]))
    (*len)--;
}

template<typename CHAR>
inline int CountConsecutiveSlashes(const CHAR *str,
                                   int begin_offset, int str_len) {
  int count = 0;
  while (begin_offset + count < str_len &&
         IsURLSlash(str[begin_offset + count]))
    ++count;
  return count;
}

void ParsePathInternal(const char* spec,
                       const Component& path,
                       Component* filepath,
                       Component* query,
                       Component* ref);
void ParsePathInternal(const char16* spec,
                       const Component& path,
                       Component* filepath,
                       Component* query,
                       Component* ref);


void ParseAfterScheme(const char* spec,
                      int spec_len,
                      int after_scheme,
                      Parsed* parsed);
void ParseAfterScheme(const char16* spec,
                      int spec_len,
                      int after_scheme,
                      Parsed* parsed);

}  

#endif  
