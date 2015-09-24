// Copyright 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_INTERNAL_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_INTERNAL_H_

#include <gtest/internal/gtest-port.h>

#ifdef GTEST_OS_LINUX
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif  

#include <iomanip>  
#include <limits>   

#include <gtest/internal/gtest-string.h>
#include <gtest/internal/gtest-filepath.h>

#define GTEST_CONCAT_TOKEN(foo, bar) GTEST_CONCAT_TOKEN_IMPL(foo, bar)
#define GTEST_CONCAT_TOKEN_IMPL(foo, bar) foo ## bar

template <typename T>
inline void GTestStreamToHelper(std::ostream* os, const T& val) {
  *os << val;
}

namespace testing {


class Message;                         
class TestCase;                        
class TestPartResult;                  
class TestInfo;                        
class UnitTest;                        
class UnitTestEventListenerInterface;  
class AssertionResult;                 

namespace internal {

struct TraceInfo;                      
class ScopedTrace;                     
class TestInfoImpl;                    
class TestResult;                      
class UnitTestImpl;                    

template <typename E> class List;      
template <typename E> class ListNode;  

class Secret;

char IsNullLiteralHelper(Secret* p);
char (&IsNullLiteralHelper(...))[2];  

#ifdef __SYMBIAN32__  
#define GTEST_IS_NULL_LITERAL(x) false
#else  
#define GTEST_IS_NULL_LITERAL(x) \
    (sizeof(::testing::internal::IsNullLiteralHelper(x)) == 1)
#endif  

String AppendUserMessage(const String& gtest_msg,
                         const Message& user_msg);

class ScopedTrace {
 public:
  
  
  ScopedTrace(const char* file, int line, const Message& message);

  
  
  
  
  ~ScopedTrace();

 private:
  GTEST_DISALLOW_COPY_AND_ASSIGN(ScopedTrace);
} GTEST_ATTRIBUTE_UNUSED;  
                           
                           

template <typename T>
String StreamableToString(const T& streamable);


#ifdef __SYMBIAN32__


template <typename T>
inline String FormatValueForFailureMessage(internal::true_type dummy,
                                           T* pointer) {
  return StreamableToString(static_cast<const void*>(pointer));
}

template <typename T>
inline String FormatValueForFailureMessage(internal::false_type dummy,
                                           const T& value) {
  return StreamableToString(value);
}

template <typename T>
inline String FormatForFailureMessage(const T& value) {
  return FormatValueForFailureMessage(
      typename internal::is_pointer<T>::type(), value);
}

#else

template <typename T>
inline String FormatForFailureMessage(const T& value) {
  return StreamableToString(value);
}

template <typename T>
inline String FormatForFailureMessage(T* pointer) {
  return StreamableToString(static_cast<const void*>(pointer));
}

#endif  

String FormatForFailureMessage(char ch);
String FormatForFailureMessage(wchar_t wchar);


#define GTEST_FORMAT_IMPL(operand2_type, operand1_printer)\
inline String FormatForComparisonFailureMessage(\
    operand2_type::value_type* str, const operand2_type& ) {\
  return operand1_printer(str);\
}\
inline String FormatForComparisonFailureMessage(\
    const operand2_type::value_type* str, const operand2_type& ) {\
  return operand1_printer(str);\
}

#if GTEST_HAS_STD_STRING
GTEST_FORMAT_IMPL(::std::string, String::ShowCStringQuoted)
#endif  
#if GTEST_HAS_STD_WSTRING
GTEST_FORMAT_IMPL(::std::wstring, String::ShowWideCStringQuoted)
#endif  

#if GTEST_HAS_GLOBAL_STRING
GTEST_FORMAT_IMPL(::string, String::ShowCStringQuoted)
#endif  
#if GTEST_HAS_GLOBAL_WSTRING
GTEST_FORMAT_IMPL(::wstring, String::ShowWideCStringQuoted)
#endif  

#undef GTEST_FORMAT_IMPL

AssertionResult EqFailure(const char* expected_expression,
                          const char* actual_expression,
                          const String& expected_value,
                          const String& actual_value,
                          bool ignoring_case);


template <typename RawType>
class FloatingPoint {
 public:
  
  
  typedef typename TypeWithSize<sizeof(RawType)>::UInt Bits;

  

  
  static const size_t kBitCount = 8*sizeof(RawType);

  
  static const size_t kFractionBitCount =
    std::numeric_limits<RawType>::digits - 1;

  
  static const size_t kExponentBitCount = kBitCount - 1 - kFractionBitCount;

  
  static const Bits kSignBitMask = static_cast<Bits>(1) << (kBitCount - 1);

  
  static const Bits kFractionBitMask =
    ~static_cast<Bits>(0) >> (kExponentBitCount + 1);

  
  static const Bits kExponentBitMask = ~(kSignBitMask | kFractionBitMask);

  
  
  
  
  
  
  
  
  
  
  
  
  static const size_t kMaxUlps = 4;

  
  
  
  
  
  
  explicit FloatingPoint(const RawType& x) : value_(x) {}

  

  
  
  
  static RawType ReinterpretBits(const Bits bits) {
    FloatingPoint fp(0);
    fp.bits_ = bits;
    return fp.value_;
  }

  
  static RawType Infinity() {
    return ReinterpretBits(kExponentBitMask);
  }

  

  
  const Bits &bits() const { return bits_; }

  
  Bits exponent_bits() const { return kExponentBitMask & bits_; }

  
  Bits fraction_bits() const { return kFractionBitMask & bits_; }

  
  Bits sign_bit() const { return kSignBitMask & bits_; }

  
  bool is_nan() const {
    
    
    return (exponent_bits() == kExponentBitMask) && (fraction_bits() != 0);
  }

  
  
  
  
  
  
  bool AlmostEquals(const FloatingPoint& rhs) const {
    
    
    if (is_nan() || rhs.is_nan()) return false;

    return DistanceBetweenSignAndMagnitudeNumbers(bits_, rhs.bits_) <= kMaxUlps;
  }

 private:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static Bits SignAndMagnitudeToBiased(const Bits &sam) {
    if (kSignBitMask & sam) {
      
      return ~sam + 1;
    } else {
      
      return kSignBitMask | sam;
    }
  }

  
  
  static Bits DistanceBetweenSignAndMagnitudeNumbers(const Bits &sam1,
                                                     const Bits &sam2) {
    const Bits biased1 = SignAndMagnitudeToBiased(sam1);
    const Bits biased2 = SignAndMagnitudeToBiased(sam2);
    return (biased1 >= biased2) ? (biased1 - biased2) : (biased2 - biased1);
  }

  union {
    RawType value_;  
    Bits bits_;      
  };
};

typedef FloatingPoint<float> Float;
typedef FloatingPoint<double> Double;

typedef void* TypeId;

template <typename T>
inline TypeId GetTypeId() {
  static bool dummy = false;
  
  
  
  return &dummy;
}

#ifdef GTEST_OS_WINDOWS

AssertionResult IsHRESULTSuccess(const char* expr, long hr);  
AssertionResult IsHRESULTFailure(const char* expr, long hr);  

#endif  

}  
}  

#define GTEST_MESSAGE(message, result_type) \
  ::testing::internal::AssertHelper(result_type, __FILE__, __LINE__, message) \
    = ::testing::Message()

#define GTEST_FATAL_FAILURE(message) \
  return GTEST_MESSAGE(message, ::testing::TPRT_FATAL_FAILURE)

#define GTEST_NONFATAL_FAILURE(message) \
  GTEST_MESSAGE(message, ::testing::TPRT_NONFATAL_FAILURE)

#define GTEST_SUCCESS(message) \
  GTEST_MESSAGE(message, ::testing::TPRT_SUCCESS)

#define GTEST_TEST_BOOLEAN(boolexpr, booltext, actual, expected, fail) \
  GTEST_AMBIGUOUS_ELSE_BLOCKER \
  if (boolexpr) \
    ; \
  else \
    fail("Value of: " booltext "\n  Actual: " #actual "\nExpected: " #expected)

#define GTEST_TEST(test_case_name, test_name, parent_class)\
class test_case_name##_##test_name##_Test : public parent_class {\
 public:\
  test_case_name##_##test_name##_Test() {}\
  static ::testing::Test* NewTest() {\
    return new test_case_name##_##test_name##_Test;\
  }\
 private:\
  virtual void TestBody();\
  static ::testing::TestInfo* const test_info_;\
  GTEST_DISALLOW_COPY_AND_ASSIGN(test_case_name##_##test_name##_Test);\
};\
\
::testing::TestInfo* const test_case_name##_##test_name##_Test::test_info_ =\
  ::testing::TestInfo::MakeAndRegisterInstance(\
    #test_case_name, \
    #test_name, \
    ::testing::internal::GetTypeId< parent_class >(), \
    parent_class::SetUpTestCase, \
    parent_class::TearDownTestCase, \
    test_case_name##_##test_name##_Test::NewTest);\
void test_case_name##_##test_name##_Test::TestBody()


#endif  
