// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_URL_UTIL_H_
#define URL_URL_UTIL_H_

#include <string>

#include "base/strings/string16.h"
#include "url/url_canon.h"
#include "url/url_export.h"
#include "url/url_parse.h"

namespace url_util {


URL_EXPORT void Initialize();

URL_EXPORT void Shutdown();


URL_EXPORT void AddStandardScheme(const char* new_scheme);

URL_EXPORT void LockStandardSchemes();

URL_EXPORT bool FindAndCompareScheme(const char* str,
                                     int str_len,
                                     const char* compare,
                                     url_parse::Component* found_scheme);
URL_EXPORT bool FindAndCompareScheme(const base::char16* str,
                                     int str_len,
                                     const char* compare,
                                     url_parse::Component* found_scheme);
inline bool FindAndCompareScheme(const std::string& str,
                                 const char* compare,
                                 url_parse::Component* found_scheme) {
  return FindAndCompareScheme(str.data(), static_cast<int>(str.size()),
                              compare, found_scheme);
}
inline bool FindAndCompareScheme(const base::string16& str,
                                 const char* compare,
                                 url_parse::Component* found_scheme) {
  return FindAndCompareScheme(str.data(), static_cast<int>(str.size()),
                              compare, found_scheme);
}

URL_EXPORT bool IsStandard(const char* spec,
                           const url_parse::Component& scheme);
URL_EXPORT bool IsStandard(const base::char16* spec,
                           const url_parse::Component& scheme);

inline bool IsStandard(const char* spec, int spec_len,
                       const url_parse::Component& scheme) {
  return IsStandard(spec, scheme);
}


URL_EXPORT bool Canonicalize(const char* spec,
                             int spec_len,
                             bool trim_path_end,
                             url_canon::CharsetConverter* charset_converter,
                             url_canon::CanonOutput* output,
                             url_parse::Parsed* output_parsed);
URL_EXPORT bool Canonicalize(const base::char16* spec,
                             int spec_len,
                             bool trim_path_end,
                             url_canon::CharsetConverter* charset_converter,
                             url_canon::CanonOutput* output,
                             url_parse::Parsed* output_parsed);

URL_EXPORT bool ResolveRelative(const char* base_spec,
                                int base_spec_len,
                                const url_parse::Parsed& base_parsed,
                                const char* relative,
                                int relative_length,
                                url_canon::CharsetConverter* charset_converter,
                                url_canon::CanonOutput* output,
                                url_parse::Parsed* output_parsed);
URL_EXPORT bool ResolveRelative(const char* base_spec,
                                int base_spec_len,
                                const url_parse::Parsed& base_parsed,
                                const base::char16* relative,
                                int relative_length,
                                url_canon::CharsetConverter* charset_converter,
                                url_canon::CanonOutput* output,
                                url_parse::Parsed* output_parsed);

// is written to output and out_parsed.
URL_EXPORT bool ReplaceComponents(
    const char* spec,
    int spec_len,
    const url_parse::Parsed& parsed,
    const url_canon::Replacements<char>& replacements,
    url_canon::CharsetConverter* charset_converter,
    url_canon::CanonOutput* output,
    url_parse::Parsed* out_parsed);
URL_EXPORT bool ReplaceComponents(
    const char* spec,
    int spec_len,
    const url_parse::Parsed& parsed,
    const url_canon::Replacements<base::char16>& replacements,
    url_canon::CharsetConverter* charset_converter,
    url_canon::CanonOutput* output,
    url_parse::Parsed* out_parsed);


URL_EXPORT bool LowerCaseEqualsASCII(const char* a_begin,
                                     const char* a_end,
                                     const char* b);
URL_EXPORT bool LowerCaseEqualsASCII(const char* a_begin,
                                     const char* a_end,
                                     const char* b_begin,
                                     const char* b_end);
URL_EXPORT bool LowerCaseEqualsASCII(const base::char16* a_begin,
                                     const base::char16* a_end,
                                     const char* b);

URL_EXPORT void DecodeURLEscapeSequences(const char* input, int length,
                                         url_canon::CanonOutputW* output);

URL_EXPORT void EncodeURIComponent(const char* input, int length,
                                   url_canon::CanonOutput* output);


}  

#endif  
