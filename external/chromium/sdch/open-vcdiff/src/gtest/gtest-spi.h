// Copyright 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GTEST_INCLUDE_GTEST_GTEST_SPI_H_
#define GTEST_INCLUDE_GTEST_GTEST_SPI_H_

#include <gtest/gtest.h>

namespace testing {

class TestPartResult {
 public:
  
  
  
  TestPartResult(TestPartResultType type,
                 const char* file_name,
                 int line_number,
                 const char* message)
      : type_(type),
        file_name_(file_name),
        line_number_(line_number),
        message_(message) {
  }

  
  TestPartResultType type() const { return type_; }

  
  
  const char* file_name() const { return file_name_.c_str(); }

  
  
  int line_number() const { return line_number_; }

  
  const char* message() const { return message_.c_str(); }

  
  bool passed() const { return type_ == TPRT_SUCCESS; }

  
  bool failed() const { return type_ != TPRT_SUCCESS; }

  
  bool nonfatally_failed() const { return type_ == TPRT_NONFATAL_FAILURE; }

  
  bool fatally_failed() const { return type_ == TPRT_FATAL_FAILURE; }
 private:
  TestPartResultType type_;

  
  
  internal::String file_name_;
  
  
  int line_number_;
  internal::String message_;  
};

std::ostream& operator<<(std::ostream& os, const TestPartResult& result);

class TestPartResultArray {
 public:
  TestPartResultArray();
  ~TestPartResultArray();

  
  void Append(const TestPartResult& result);

  
  const TestPartResult& GetTestPartResult(int index) const;

  
  int size() const;
 private:
  
  
  internal::List<TestPartResult>* const list_;

  GTEST_DISALLOW_COPY_AND_ASSIGN(TestPartResultArray);
};

class TestPartResultReporterInterface {
 public:
  virtual ~TestPartResultReporterInterface() {}

  virtual void ReportTestPartResult(const TestPartResult& result) = 0;
};

class ScopedFakeTestPartResultReporter
    : public TestPartResultReporterInterface {
 public:
  
  
  
  explicit ScopedFakeTestPartResultReporter(TestPartResultArray* result);

  
  virtual ~ScopedFakeTestPartResultReporter();

  
  
  
  
  
  virtual void ReportTestPartResult(const TestPartResult& result);
 private:
  TestPartResultReporterInterface* const old_reporter_;
  TestPartResultArray* const result_;

  GTEST_DISALLOW_COPY_AND_ASSIGN(ScopedFakeTestPartResultReporter);
};

namespace internal {

class SingleFailureChecker {
 public:
  
  SingleFailureChecker(const TestPartResultArray* results,
                       TestPartResultType type,
                       const char* substr);
  ~SingleFailureChecker();
 private:
  const TestPartResultArray* const results_;
  const TestPartResultType type_;
  const String substr_;

  GTEST_DISALLOW_COPY_AND_ASSIGN(SingleFailureChecker);
};

}  

}  

#define EXPECT_FATAL_FAILURE(statement, substr) do {\
    class GTestExpectFatalFailureHelper {\
     public:\
      static void Execute() { statement; }\
    };\
    ::testing::TestPartResultArray gtest_failures;\
    ::testing::internal::SingleFailureChecker gtest_checker(\
        &gtest_failures, ::testing::TPRT_FATAL_FAILURE, (substr));\
    {\
      ::testing::ScopedFakeTestPartResultReporter gtest_reporter(\
          &gtest_failures);\
      GTestExpectFatalFailureHelper::Execute();\
    }\
  } while (false)

#define EXPECT_NONFATAL_FAILURE(statement, substr) do {\
    ::testing::TestPartResultArray gtest_failures;\
    ::testing::internal::SingleFailureChecker gtest_checker(\
        &gtest_failures, ::testing::TPRT_NONFATAL_FAILURE, (substr));\
    {\
      ::testing::ScopedFakeTestPartResultReporter gtest_reporter(\
          &gtest_failures);\
      statement;\
    }\
  } while (false)

#endif  
