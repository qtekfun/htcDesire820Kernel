// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_URL_PARSE_INTERNAL_H_
#define URL_URL_PARSE_INTERNAL_H_


#include "url/url_parse.h"

namespace url_parse {

inline bool IsURLSlash(base::char16 ch) {
  return ch == '/' || ch == '\\';
}

inline bool ShouldTrimFromURL(base::char16 ch) {
  return ch <= ' ';
}

template<typename CHAR>
inline void TrimURL(const CHAR* spec, int* begin, int* len,
                    bool trim_path_end = true) {
  
  while (*begin < *len && ShouldTrimFromURL(spec[*begin]))
    (*begin)++;

  if (trim_path_end) {
    
    
    
    while (*len > *begin && ShouldTrimFromURL(spec[*len - 1]))
      (*len)--;
  }
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
void ParsePathInternal(const base::char16* spec,
                       const Component& path,
                       Component* filepath,
                       Component* query,
                       Component* ref);


void ParseAfterScheme(const char* spec,
                      int spec_len,
                      int after_scheme,
                      Parsed* parsed);
void ParseAfterScheme(const base::char16* spec,
                      int spec_len,
                      int after_scheme,
                      Parsed* parsed);

}  

#endif  
