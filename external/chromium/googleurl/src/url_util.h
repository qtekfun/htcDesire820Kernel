// Copyright 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GOOGLEURL_SRC_URL_UTIL_H__
#define GOOGLEURL_SRC_URL_UTIL_H__

#include <string>

#include "base/string16.h"
#include "googleurl/src/url_common.h"
#include "googleurl/src/url_parse.h"
#include "googleurl/src/url_canon.h"

namespace url_util {


GURL_API void Initialize();

GURL_API void Shutdown();


GURL_API void AddStandardScheme(const char* new_scheme);

GURL_API void LockStandardSchemes();

GURL_API bool FindAndCompareScheme(const char* str,
                                   int str_len,
                                   const char* compare,
                                   url_parse::Component* found_scheme);
GURL_API bool FindAndCompareScheme(const char16* str,
                                   int str_len,
                                   const char* compare,
                                   url_parse::Component* found_scheme);
inline bool FindAndCompareScheme(const std::string& str,
                                 const char* compare,
                                 url_parse::Component* found_scheme) {
  return FindAndCompareScheme(str.data(), static_cast<int>(str.size()),
                              compare, found_scheme);
}
inline bool FindAndCompareScheme(const string16& str,
                                 const char* compare,
                                 url_parse::Component* found_scheme) {
  return FindAndCompareScheme(str.data(), static_cast<int>(str.size()),
                              compare, found_scheme);
}

GURL_API bool IsStandard(const char* spec,
                         const url_parse::Component& scheme);
GURL_API bool IsStandard(const char16* spec,
                         const url_parse::Component& scheme);

inline bool IsStandard(const char* spec, int spec_len,
                       const url_parse::Component& scheme) {
  return IsStandard(spec, scheme);
}


GURL_API bool Canonicalize(const char* spec,
                           int spec_len,
                           url_canon::CharsetConverter* charset_converter,
                           url_canon::CanonOutput* output,
                           url_parse::Parsed* output_parsed);
GURL_API bool Canonicalize(const char16* spec,
                           int spec_len,
                           url_canon::CharsetConverter* charset_converter,
                           url_canon::CanonOutput* output,
                           url_parse::Parsed* output_parsed);

GURL_API bool ResolveRelative(const char* base_spec,
                              int base_spec_len,
                              const url_parse::Parsed& base_parsed,
                              const char* relative,
                              int relative_length,
                              url_canon::CharsetConverter* charset_converter,
                              url_canon::CanonOutput* output,
                              url_parse::Parsed* output_parsed);
GURL_API bool ResolveRelative(const char* base_spec,
                              int base_spec_len,
                              const url_parse::Parsed& base_parsed,
                              const char16* relative,
                              int relative_length,
                              url_canon::CharsetConverter* charset_converter,
                              url_canon::CanonOutput* output,
                              url_parse::Parsed* output_parsed);

// is written to output and out_parsed.
GURL_API bool ReplaceComponents(
    const char* spec,
    int spec_len,
    const url_parse::Parsed& parsed,
    const url_canon::Replacements<char>& replacements,
    url_canon::CharsetConverter* charset_converter,
    url_canon::CanonOutput* output,
    url_parse::Parsed* out_parsed);
GURL_API bool ReplaceComponents(
    const char* spec,
    int spec_len,
    const url_parse::Parsed& parsed,
    const url_canon::Replacements<char16>& replacements,
    url_canon::CharsetConverter* charset_converter,
    url_canon::CanonOutput* output,
    url_parse::Parsed* out_parsed);


GURL_API bool LowerCaseEqualsASCII(const char* a_begin,
                                   const char* a_end,
                                   const char* b);
GURL_API bool LowerCaseEqualsASCII(const char* a_begin,
                                   const char* a_end,
                                   const char* b_begin,
                                   const char* b_end);
GURL_API bool LowerCaseEqualsASCII(const char16* a_begin,
                                   const char16* a_end,
                                   const char* b);

GURL_API void DecodeURLEscapeSequences(const char* input, int length,
                                       url_canon::CanonOutputW* output);

}  

#endif  
