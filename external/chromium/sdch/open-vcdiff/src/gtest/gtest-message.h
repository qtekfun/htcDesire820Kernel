// Copyright 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GTEST_INCLUDE_GTEST_GTEST_MESSAGE_H_
#define GTEST_INCLUDE_GTEST_GTEST_MESSAGE_H_

#include <gtest/internal/gtest-string.h>
#include <gtest/internal/gtest-internal.h>

namespace testing {

class Message {
 private:
  
  
  typedef std::ostream& (*BasicNarrowIoManip)(std::ostream&);

 public:
  
  
  
  
  
  Message() : ss_(new internal::StrStream) {}

  
  Message(const Message& msg) : ss_(new internal::StrStream) {  
    *ss_ << msg.GetString();
  }

  
  explicit Message(const char* str) : ss_(new internal::StrStream) {
    *ss_ << str;
  }

  ~Message() { delete ss_; }
#ifdef __SYMBIAN32__
  
  template <typename T>
  inline Message& operator <<(const T& value) {
    StreamHelper(typename internal::is_pointer<T>::type(), value);
    return *this;
  }
#else
  
  template <typename T>
  inline Message& operator <<(const T& val) {
    ::GTestStreamToHelper(ss_, val);
    return *this;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  template <typename T>
  inline Message& operator <<(T* const& pointer) {  
    if (pointer == NULL) {
      *ss_ << "(null)";
    } else {
      ::GTestStreamToHelper(ss_, pointer);
    }
    return *this;
  }
#endif  

  
  
  
  
  
  
  Message& operator <<(BasicNarrowIoManip val) {
    *ss_ << val;
    return *this;
  }

  
  Message& operator <<(bool b) {
    return *this << (b ? "true" : "false");
  }

  
  
  Message& operator <<(const wchar_t* wide_c_str) {
    return *this << internal::String::ShowWideCString(wide_c_str);
  }
  Message& operator <<(wchar_t* wide_c_str) {
    return *this << internal::String::ShowWideCString(wide_c_str);
  }

#if GTEST_HAS_STD_WSTRING
  
  
  Message& operator <<(const ::std::wstring& wstr);
#endif  

#if GTEST_HAS_GLOBAL_WSTRING
  
  
  Message& operator <<(const ::wstring& wstr);
#endif  

  
  
  
  
  internal::String GetString() const {
    return internal::StrStreamToString(ss_);
  }

 private:
#ifdef __SYMBIAN32__
  
  
  
  
  template <typename T>
  inline void StreamHelper(internal::true_type dummy, T* pointer) {
    if (pointer == NULL) {
      *ss_ << "(null)";
    } else {
      ::GTestStreamToHelper(ss_, pointer);
    }
  }
  template <typename T>
  inline void StreamHelper(internal::false_type dummy, const T& value) {
    ::GTestStreamToHelper(ss_, value);
  }
#endif  

  
  internal::StrStream* const ss_;

  
  
  void operator=(const Message&);
};

inline std::ostream& operator <<(std::ostream& os, const Message& sb) {
  return os << sb.GetString();
}

}  

#endif  
