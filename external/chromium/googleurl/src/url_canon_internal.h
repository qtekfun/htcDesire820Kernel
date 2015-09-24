// Copyright 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLEURL_SRC_URL_CANON_INTERNAL_H__
#define GOOGLEURL_SRC_URL_CANON_INTERNAL_H__

#include <stdlib.h>

#include "base/logging.h"
#include "googleurl/src/url_canon.h"

namespace url_canon {


enum SharedCharTypes {
  
  
  CHAR_QUERY = 1,

  
  CHAR_USERINFO = 2,

  
  CHAR_IPV4 = 4,

  
  CHAR_HEX = 8,

  
  CHAR_DEC = 16,

  
  CHAR_OCT = 32,
};

extern const unsigned char kSharedCharTypeTable[0x100];

inline bool IsCharOfType(unsigned char c, SharedCharTypes type) {
  return !!(kSharedCharTypeTable[c] & type);
}
inline bool IsQueryChar(unsigned char c) {
  return IsCharOfType(c, CHAR_QUERY);
}
inline bool IsIPv4Char(unsigned char c) {
  return IsCharOfType(c, CHAR_IPV4);
}
inline bool IsHexChar(unsigned char c) {
  return IsCharOfType(c, CHAR_HEX);
}

void AppendStringOfType(const char* source, int length,
                        SharedCharTypes type,
                        CanonOutput* output);
void AppendStringOfType(const char16* source, int length,
                        SharedCharTypes type,
                        CanonOutput* output);

extern const char kHexCharLookup[0x10];

extern const char kCharToHexLookup[8];

inline unsigned char HexCharToValue(unsigned char c) {
  return c - kCharToHexLookup[c / 0x20];
}

template<typename CHAR>
inline int IsDot(const CHAR* spec, int offset, int end) {
  if (spec[offset] == '.') {
    return 1;
  } else if (spec[offset] == '%' && offset + 3 <= end &&
             spec[offset + 1] == '2' &&
             (spec[offset + 2] == 'e' || spec[offset + 2] == 'E')) {
    
    return 3;
  }
  return 0;
}

char CanonicalSchemeChar(char16 ch);

template<typename UINCHAR, typename OUTCHAR>
inline void AppendEscapedChar(UINCHAR ch,
                              CanonOutputT<OUTCHAR>* output) {
  output->push_back('%');
  output->push_back(kHexCharLookup[ch >> 4]);
  output->push_back(kHexCharLookup[ch & 0xf]);
}

extern const char16 kUnicodeReplacementCharacter;


bool ReadUTFChar(const char* str, int* begin, int length,
                 unsigned* code_point_out);

template<class Output, void Appender(unsigned char, Output*)>
inline void DoAppendUTF8(unsigned char_value, Output* output) {
  if (char_value <= 0x7f) {
    Appender(static_cast<unsigned char>(char_value), output);
  } else if (char_value <= 0x7ff) {
    
    Appender(static_cast<unsigned char>(0xC0 | (char_value >> 6)),
             output);
    Appender(static_cast<unsigned char>(0x80 | (char_value & 0x3f)),
             output);
  } else if (char_value <= 0xffff) {
    
    Appender(static_cast<unsigned char>(0xe0 | (char_value >> 12)),
             output);
    Appender(static_cast<unsigned char>(0x80 | ((char_value >> 6) & 0x3f)),
             output);
    Appender(static_cast<unsigned char>(0x80 | (char_value & 0x3f)),
             output);
  } else if (char_value <= 0x10FFFF) {  
    
    Appender(static_cast<unsigned char>(0xf0 | (char_value >> 18)),
             output);
    Appender(static_cast<unsigned char>(0x80 | ((char_value >> 12) & 0x3f)),
             output);
    Appender(static_cast<unsigned char>(0x80 | ((char_value >> 6) & 0x3f)),
             output);
    Appender(static_cast<unsigned char>(0x80 | (char_value & 0x3f)),
             output);
  } else {
    
    NOTREACHED();
  }
}

inline void AppendCharToOutput(unsigned char ch, CanonOutput* output) {
  output->push_back(static_cast<char>(ch));
}

inline void AppendUTF8Value(unsigned char_value, CanonOutput* output) {
  DoAppendUTF8<CanonOutput, AppendCharToOutput>(char_value, output);
}

inline void AppendUTF8EscapedValue(unsigned char_value, CanonOutput* output) {
  DoAppendUTF8<CanonOutput, AppendEscapedChar>(char_value, output);
}


bool ReadUTFChar(const char16* str, int* begin, int length,
                 unsigned* code_point);

inline void AppendUTF16Value(unsigned code_point,
                             CanonOutputT<char16>* output) {
  if (code_point > 0xffff) {
    output->push_back(static_cast<char16>((code_point >> 10) + 0xd7c0));
    output->push_back(static_cast<char16>((code_point & 0x3ff) | 0xdc00));
  } else {
    output->push_back(static_cast<char16>(code_point));
  }
}


inline bool AppendUTF8EscapedChar(const char16* str, int* begin, int length,
                                  CanonOutput* output) {
  
  
  
  unsigned char_value;
  bool success = ReadUTFChar(str, begin, length, &char_value);
  AppendUTF8EscapedValue(char_value, output);
  return success;
}

inline bool AppendUTF8EscapedChar(const char* str, int* begin, int length,
                                  CanonOutput* output) {
  
  
  
  unsigned ch;
  bool success = ReadUTFChar(str, begin, length, &ch);
  AppendUTF8EscapedValue(ch, output);
  return success;
}

inline bool Is8BitChar(char c) {
  return true;  
}
inline bool Is8BitChar(char16 c) {
  return c <= 255;
}

template<typename CHAR>
inline bool DecodeEscaped(const CHAR* spec, int* begin, int end,
                          unsigned char* unescaped_value) {
  if (*begin + 3 > end ||
      !Is8BitChar(spec[*begin + 1]) || !Is8BitChar(spec[*begin + 2])) {
    
    
    return false;
  }

  unsigned char first = static_cast<unsigned char>(spec[*begin + 1]);
  unsigned char second = static_cast<unsigned char>(spec[*begin + 2]);
  if (!IsHexChar(first) || !IsHexChar(second)) {
    
    return false;
  }

  
  *unescaped_value = (HexCharToValue(first) << 4) + HexCharToValue(second);
  *begin += 2;
  return true;
}

void AppendInvalidNarrowString(const char* spec, int begin, int end,
                               CanonOutput* output);
void AppendInvalidNarrowString(const char16* spec, int begin, int end,
                               CanonOutput* output);


bool ConvertUTF16ToUTF8(const char16* input, int input_len,
                        CanonOutput* output);
bool ConvertUTF8ToUTF16(const char* input, int input_len,
                        CanonOutputT<char16>* output);

void ConvertUTF16ToQueryEncoding(const char16* input,
                                 const url_parse::Component& query,
                                 CharsetConverter* converter,
                                 CanonOutput* output);

void SetupOverrideComponents(const char* base,
                             const Replacements<char>& repl,
                             URLComponentSource<char>* source,
                             url_parse::Parsed* parsed);

bool SetupUTF16OverrideComponents(const char* base,
                                  const Replacements<char16>& repl,
                                  CanonOutput* utf8_buffer,
                                  URLComponentSource<char>* source,
                                  url_parse::Parsed* parsed);

bool CanonicalizePartialPath(const char* spec,
                             const url_parse::Component& path,
                             int path_begin_in_output,
                             CanonOutput* output);
bool CanonicalizePartialPath(const char16* spec,
                             const url_parse::Component& path,
                             int path_begin_in_output,
                             CanonOutput* output);

#ifndef WIN32

int _itoa_s(int value, char* buffer, size_t size_in_chars, int radix);
int _itow_s(int value, char16* buffer, size_t size_in_chars, int radix);

template<size_t N>
inline int _itoa_s(int value, char (&buffer)[N], int radix) {
  return _itoa_s(value, buffer, N, radix);
}

template<size_t N>
inline int _itow_s(int value, char16 (&buffer)[N], int radix) {
  return _itow_s(value, buffer, N, radix);
}

inline unsigned long long _strtoui64(const char* nptr,
                                     char** endptr, int base) {
  return strtoull(nptr, endptr, base);
}

#endif  

}  

#endif  
