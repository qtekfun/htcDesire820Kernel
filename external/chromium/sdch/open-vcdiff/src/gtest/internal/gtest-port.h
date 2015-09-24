// Copyright 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_PORT_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_PORT_H_



#include <stdlib.h>
#include <stdio.h>

#define GTEST_NAME "Google Test"
#define GTEST_FLAG_PREFIX "gtest_"
#define GTEST_FLAG_PREFIX_UPPER "GTEST_"

#ifdef __CYGWIN__
#define GTEST_OS_CYGWIN
#elif defined _MSC_VER
#define GTEST_OS_WINDOWS
#elif defined __APPLE__
#define GTEST_OS_MAC
#elif defined __linux__
#define GTEST_OS_LINUX
#endif  


#ifndef GTEST_HAS_STD_STRING

#ifdef GTEST_OS_WINDOWS
#ifndef _HAS_EXCEPTIONS
#define _HAS_EXCEPTIONS 1
#endif  
#define GTEST_HAS_EXCEPTIONS _HAS_EXCEPTIONS
#define GTEST_HAS_STD_STRING ((_MSC_VER >= 1400) || GTEST_HAS_EXCEPTIONS)
#else  
#define GTEST_HAS_EXCEPTIONS 0
#define GTEST_HAS_STD_STRING 1
#endif  

#endif  

#ifndef GTEST_HAS_GLOBAL_STRING

#define GTEST_HAS_GLOBAL_STRING 0

#endif  

#ifndef GTEST_HAS_STD_WSTRING

#ifdef GTEST_OS_CYGWIN
#define GTEST_HAS_STD_WSTRING 0
#else
#define GTEST_HAS_STD_WSTRING GTEST_HAS_STD_STRING
#endif  

#endif  

#ifndef GTEST_HAS_GLOBAL_WSTRING
#define GTEST_HAS_GLOBAL_WSTRING GTEST_HAS_GLOBAL_STRING
#endif  

#if GTEST_HAS_STD_STRING || GTEST_HAS_GLOBAL_STRING || \
    GTEST_HAS_STD_WSTRING || GTEST_HAS_GLOBAL_WSTRING
#include <string>  
#endif  
        

#if GTEST_HAS_STD_STRING
#include <sstream>  
#else
#include <strstream>  
#endif  

#if GTEST_HAS_STD_STRING && defined(GTEST_OS_LINUX)
#define GTEST_HAS_DEATH_TEST
#include <regex.h>
#include <vector>
#include <fcntl.h>
#include <sys/mman.h>
#endif  


#ifdef __INTEL_COMPILER
#define GTEST_AMBIGUOUS_ELSE_BLOCKER
#else
#define GTEST_AMBIGUOUS_ELSE_BLOCKER switch (0) case 0:  
#endif

#if defined(GTEST_OS_WINDOWS) || (defined(GTEST_OS_LINUX) && defined(SWIG))
#define GTEST_ATTRIBUTE_UNUSED
#else
#define GTEST_ATTRIBUTE_UNUSED __attribute__ ((unused))
#endif  

#define GTEST_DISALLOW_COPY_AND_ASSIGN(type)\
  type(const type &);\
  void operator=(const type &)

#if defined(__GNUC__) \
  && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)) \
  && !defined(COMPILER_ICC)
#define GTEST_MUST_USE_RESULT __attribute__ ((warn_unused_result))
#else
#define GTEST_MUST_USE_RESULT
#endif  

namespace testing {

class Message;

namespace internal {

class String;

#if GTEST_HAS_STD_STRING
typedef ::std::stringstream StrStream;
#else
typedef ::std::strstream StrStream;
#endif  


template <typename T>
class scoped_ptr {
 public:
  explicit scoped_ptr(T* p = NULL) : ptr_(p) {}
  ~scoped_ptr() { reset(); }

  T& operator*() const { return *ptr_; }
  T* operator->() const { return ptr_; }
  T* get() const { return ptr_; }

  T* release() {
    T* const ptr = ptr_;
    ptr_ = NULL;
    return ptr;
  }

  void reset(T* p = NULL) {
    if (p != ptr_) {
      if (sizeof(T) > 0) {  
        delete ptr_;
      }
      ptr_ = p;
    }
  }
 private:
  T* ptr_;

  GTEST_DISALLOW_COPY_AND_ASSIGN(scoped_ptr);
};

#ifdef GTEST_HAS_DEATH_TEST


class RE {
 public:
  
#if GTEST_HAS_STD_STRING
  RE(const ::std::string& regex) { Init(regex.c_str()); }  
#endif  

#if GTEST_HAS_GLOBAL_STRING
  RE(const ::string& regex) { Init(regex.c_str()); }  
#endif  

  RE(const char* regex) { Init(regex); }  
  ~RE();

  
  const char* pattern() const { return pattern_; }

  

  
  
#if GTEST_HAS_STD_STRING
  static bool PartialMatch(const ::std::string& str, const RE& re) {
    return PartialMatch(str.c_str(), re);
  }
#endif  

#if GTEST_HAS_GLOBAL_STRING
  static bool PartialMatch(const ::string& str, const RE& re) {
    return PartialMatch(str.c_str(), re);
  }
#endif  

  static bool PartialMatch(const char* str, const RE& re);

 private:
  void Init(const char* regex);

  
  
  
  
  const char* pattern_;
  regex_t regex_;
  bool is_valid_;
};

#endif  


enum GTestLogSeverity {
  GTEST_INFO,
  GTEST_WARNING,
  GTEST_ERROR,
  GTEST_FATAL
};

void GTestLog(GTestLogSeverity severity, const char* file,
              int line, const char* msg);

#define GTEST_LOG(severity, msg)\
    ::testing::internal::GTestLog(\
        ::testing::internal::GTEST_##severity, __FILE__, __LINE__, \
        (::testing::Message() << (msg)).GetString().c_str())

inline void LogToStderr() {}
inline void FlushInfoLog() { fflush(NULL); }


#ifdef GTEST_HAS_DEATH_TEST

extern ::std::vector<String> g_argvs;

void CaptureStderr();
::std::string GetCapturedStderr();
const ::std::vector<String>& GetArgvs();

#endif  



class Mutex {
 public:
  Mutex() {}
  explicit Mutex(int ) {}
  void AssertHeld() const {}
  enum { NO_CONSTRUCTOR_NEEDED_FOR_STATIC_MUTEX = 0 };
};

class GTestMutexLock {
 public:
  explicit GTestMutexLock(Mutex*) {}  
};

typedef GTestMutexLock MutexLock;

template <typename T>
class ThreadLocal {
 public:
  T* pointer() { return &value_; }
  const T* pointer() const { return &value_; }
  const T& get() const { return value_; }
  void set(const T& value) { value_ = value; }
 private:
  T value_;
};

inline size_t GetThreadCount() { return 0; }


#ifdef __SYMBIAN32__


template <bool bool_value>
struct bool_constant {
  typedef bool_constant<bool_value> type;
  static const bool value = bool_value;
};
template <bool bool_value> const bool bool_constant<bool_value>::value;

typedef bool_constant<false> false_type;
typedef bool_constant<true> true_type;

template <typename T>
struct is_pointer : public false_type {};

template <typename T>
struct is_pointer<T*> : public true_type {};

#endif  


#ifdef GTEST_OS_WINDOWS
typedef __int64 BiggestInt;
#else
typedef long long BiggestInt;  
#endif  

const BiggestInt kMaxBiggestInt =
    ~(static_cast<BiggestInt>(1) << (8*sizeof(BiggestInt) - 1));

template <size_t size>
class TypeWithSize {
 public:
  
  
  typedef void UInt;
};

template <>
class TypeWithSize<4> {
 public:
  
  
  
  
  typedef int Int;
  typedef unsigned int UInt;
};

template <>
class TypeWithSize<8> {
 public:
#ifdef GTEST_OS_WINDOWS
  typedef __int64 Int;
  typedef unsigned __int64 UInt;
#else
  typedef long long Int;  
  typedef unsigned long long UInt;  
#endif  
};

typedef TypeWithSize<4>::Int Int32;
typedef TypeWithSize<4>::UInt UInt32;
typedef TypeWithSize<8>::Int Int64;
typedef TypeWithSize<8>::UInt UInt64;
typedef TypeWithSize<8>::Int TimeInMillis;  


inline const char* GetEnv(const char* name) {
#ifdef _WIN32_WCE  
  
  return NULL;
#elif defined(GTEST_OS_WINDOWS)  
  
  
#pragma warning(push)          
#pragma warning(disable:4996)  
  return getenv(name);
#pragma warning(pop)           
#else  
  return getenv(name);
#endif
}

#define GTEST_FLAG(name) FLAGS_gtest_##name

#define GTEST_DECLARE_bool(name) extern bool GTEST_FLAG(name)
#define GTEST_DECLARE_int32(name) \
    extern ::testing::internal::Int32 GTEST_FLAG(name)
#define GTEST_DECLARE_string(name) \
    extern ::testing::internal::String GTEST_FLAG(name)

#define GTEST_DEFINE_bool(name, default_val, doc) \
    bool GTEST_FLAG(name) = (default_val)
#define GTEST_DEFINE_int32(name, default_val, doc) \
    ::testing::internal::Int32 GTEST_FLAG(name) = (default_val)
#define GTEST_DEFINE_string(name, default_val, doc) \
    ::testing::internal::String GTEST_FLAG(name) = (default_val)

bool ParseInt32(const Message& src_text, const char* str, Int32* value);

bool BoolFromGTestEnv(const char* flag, bool default_val);
Int32 Int32FromGTestEnv(const char* flag, Int32 default_val);
const char* StringFromGTestEnv(const char* flag, const char* default_val);

}  
}  

#endif  
