/*
 * libjingle
 * Copyright 2011, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TALK_BASE_STRINGENCODE_H__
#define TALK_BASE_STRINGENCODE_H__

#include <string>
#include <sstream>
#include <vector>

namespace talk_base {


char hex_encode(unsigned char val);
bool hex_decode(char ch, unsigned char* val);

size_t utf8_encode(char* buffer, size_t buflen, unsigned long value);
size_t utf8_decode(const char* source, size_t srclen, unsigned long* value);

size_t escape(char * buffer, size_t buflen,
              const char * source, size_t srclen,
              const char * illegal, char escape);
size_t unescape(char * buffer, size_t buflen,
                const char * source, size_t srclen,
                char escape);

size_t encode(char * buffer, size_t buflen,
              const char * source, size_t srclen,
              const char * illegal, char escape);
size_t decode(char * buffer, size_t buflen,
              const char * source, size_t srclen,
              char escape);

const char* unsafe_filename_characters();

size_t url_encode(char * buffer, size_t buflen,
                  const char * source, size_t srclen);
size_t url_decode(char * buffer, size_t buflen,
                  const char * source, size_t srclen);

size_t html_encode(char * buffer, size_t buflen,
                   const char * source, size_t srclen);
size_t html_decode(char * buffer, size_t buflen,
                   const char * source, size_t srclen);

size_t xml_encode(char * buffer, size_t buflen,
                  const char * source, size_t srclen);
size_t xml_decode(char * buffer, size_t buflen,
                  const char * source, size_t srclen);

size_t hex_encode(char * buffer, size_t buflen,
                  const char * source, size_t srclen);
size_t hex_decode(char * buffer, size_t buflen,
                  const char * source, size_t srclen);
std::string hex_encode(const char * source, size_t srclen);

typedef size_t (*Transform)(char * buffer, size_t buflen,
                            const char * source, size_t srclen);
size_t transform(std::string& value, size_t maxlen, const std::string& source,
                 Transform t);

std::string s_transform(const std::string& source, Transform t);

inline std::string s_url_encode(const std::string& source) {
  return s_transform(source, url_encode);
}
inline std::string s_url_decode(const std::string& source) {
  return s_transform(source, url_decode);
}

size_t split(const std::string& source, char delimiter,
             std::vector<std::string>* fields);

size_t tokenize(const std::string& source, char delimiter,
                std::vector<std::string>* fields);



template <class T>
static bool ToString(const T &t, std::string* s) {
  std::ostringstream oss;
  oss << t;
  *s = oss.str();
  return !oss.fail();
}

template <class T>
static bool FromString(const std::string& s, T* t) {
  std::istringstream iss(s);
  iss >> *t;
  return !iss.fail();
}


template<typename T>
static inline std::string ToString(T val) {
  std::string str; ToString(val, &str); return str;
}

template<typename T>
static inline T FromString(const std::string& str) {
  T val; FromString(str, &val); return val;
}

template<typename T>
static inline T FromString(const T& defaultValue, const std::string& str) {
  T val(defaultValue); FromString(str, &val); return val;
}

char make_char_safe_for_filename(char c);


}  

#endif  
