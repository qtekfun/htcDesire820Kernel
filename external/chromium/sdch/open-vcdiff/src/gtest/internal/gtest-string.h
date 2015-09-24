// Copyright 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_STRING_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_STRING_H_

#include <string.h>
#include <gtest/internal/gtest-port.h>

namespace testing {
namespace internal {

class String {
 public:
  

  
  
  
  
  
  
  
  
  
  static inline const char* ShowCString(const char* c_str) {
    return c_str ? c_str : "(null)";
  }

  
  
  
  
  
  
  
  static String ShowCStringQuoted(const char* c_str);

  
  
  
  
  
  
  
  static const char* CloneCString(const char* c_str);

  
  
  
  
  
  static bool CStringEquals(const char* lhs, const char* rhs);

  
  
  
  
  static String ShowWideCString(const wchar_t* wide_c_str);

  
  
  static String ShowWideCStringQuoted(const wchar_t* wide_c_str);

  
  
  
  
  
  
  static bool WideCStringEquals(const wchar_t* lhs, const wchar_t* rhs);

  
  
  
  
  
  
  static bool CaseInsensitiveCStringEquals(const char* lhs,
                                           const char* rhs);

  
  
  
  
  
  
  
  
  
  static String Format(const char* format, ...);

  

  
  String() : c_str_(NULL) {}

  
  String(const char* c_str) : c_str_(NULL) {  
    *this = c_str;
  }

  
  
  String(const char* buffer, size_t len);

  
  
  String(const String& str) : c_str_(NULL) {
    *this = str;
  }

  
  
  ~String() { delete[] c_str_; }

  
  bool empty() const {
    return (c_str_ != NULL) && (*c_str_ == '\0');
  }

  
  
  
  int Compare(const String& rhs) const;

  
  
  bool operator==(const char* c_str) const {
    return CStringEquals(c_str_, c_str);
  }

  
  
  bool operator!=(const char* c_str) const {
    return !CStringEquals(c_str_, c_str);
  }

  
  
  bool EndsWith(const char* suffix) const;

  
  
  bool EndsWithCaseInsensitive(const char* suffix) const;

  
  
  int GetLength() const {
    return c_str_ ? static_cast<int>(strlen(c_str_)) : -1;
  }

  
  
  
  const char* c_str() const { return c_str_; }

  
  
  
  
  
  
  
  
  void Set(const char* c_str, size_t length);

  
  const String& operator=(const char* c_str);

  
  const String& operator=(const String &rhs) {
    *this = rhs.c_str_;
    return *this;
  }

 private:
  const char* c_str_;
};

inline ::std::ostream& operator <<(::std::ostream& os, const String& str) {
  
  
  return os << String::ShowCString(str.c_str());
}

String StrStreamToString(StrStream* stream);


template <typename T>
String StreamableToString(const T& streamable);

}  
}  

#endif  
