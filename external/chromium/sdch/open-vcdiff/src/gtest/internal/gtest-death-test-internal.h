// Copyright 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_DEATH_TEST_INTERNAL_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_DEATH_TEST_INTERNAL_H_

#include <gtest/internal/gtest-internal.h>

namespace testing {
namespace internal {

GTEST_DECLARE_string(internal_run_death_test);

const char kDeathTestStyleFlag[] = "death_test_style";
const char kInternalRunDeathTestFlag[] = "internal_run_death_test";

#ifdef GTEST_HAS_DEATH_TEST


class DeathTest {
 public:
  
  
  
  
  
  
  
  
  static bool Create(const char* statement, const RE* regex,
                     const char* file, int line, DeathTest** test);
  DeathTest();
  virtual ~DeathTest() { }

  
  class ReturnSentinel {
   public:
    explicit ReturnSentinel(DeathTest* test) : test_(test) { }
    ~ReturnSentinel() { test_->Abort(TEST_ENCOUNTERED_RETURN_STATEMENT); }
   private:
    DeathTest* const test_;
    GTEST_DISALLOW_COPY_AND_ASSIGN(ReturnSentinel);
  } GTEST_ATTRIBUTE_UNUSED;

  
  
  
  
  
  enum TestRole { OVERSEE_TEST, EXECUTE_TEST };

  
  enum AbortReason { TEST_ENCOUNTERED_RETURN_STATEMENT, TEST_DID_NOT_DIE };

  
  virtual TestRole AssumeRole() = 0;

  
  virtual int Wait() = 0;

  
  
  
  
  
  
  
  virtual bool Passed(bool exit_status_ok) = 0;

  
  virtual void Abort(AbortReason reason) = 0;

  
  
  static const char* LastMessage();

 private:
  GTEST_DISALLOW_COPY_AND_ASSIGN(DeathTest);
};

class DeathTestFactory {
 public:
  virtual ~DeathTestFactory() { }
  virtual bool Create(const char* statement, const RE* regex,
                      const char* file, int line, DeathTest** test) = 0;
};

class DefaultDeathTestFactory : public DeathTestFactory {
 public:
  virtual bool Create(const char* statement, const RE* regex,
                      const char* file, int line, DeathTest** test);
};

bool ExitedUnsuccessfully(int exit_status);

#define GTEST_DEATH_TEST(statement, predicate, regex, fail) \
  GTEST_AMBIGUOUS_ELSE_BLOCKER \
  if (true) { \
    const ::testing::internal::RE& gtest_regex = (regex); \
    ::testing::internal::DeathTest* gtest_dt; \
    if (!::testing::internal::DeathTest::Create(#statement, &gtest_regex, \
        __FILE__, __LINE__, &gtest_dt)) { \
      goto GTEST_CONCAT_TOKEN(gtest_label_, __LINE__); \
    } \
    if (gtest_dt != NULL) { \
      ::testing::internal::scoped_ptr< ::testing::internal::DeathTest> \
          gtest_dt_ptr(gtest_dt); \
      switch (gtest_dt->AssumeRole()) { \
        case ::testing::internal::DeathTest::OVERSEE_TEST: \
          if (!gtest_dt->Passed(predicate(gtest_dt->Wait()))) { \
            goto GTEST_CONCAT_TOKEN(gtest_label_, __LINE__); \
          } \
          break; \
        case ::testing::internal::DeathTest::EXECUTE_TEST: { \
          ::testing::internal::DeathTest::ReturnSentinel \
              gtest_sentinel(gtest_dt); \
          { statement; } \
          gtest_dt->Abort(::testing::internal::DeathTest::TEST_DID_NOT_DIE); \
          break; \
        } \
      } \
    } \
  } else \
    GTEST_CONCAT_TOKEN(gtest_label_, __LINE__): \
      fail(::testing::internal::DeathTest::LastMessage())

struct InternalRunDeathTestFlag {
  String file;
  int line;
  int index;
  int status_fd;
};

InternalRunDeathTestFlag* ParseInternalRunDeathTestFlag();

#endif  

}  
}  

#endif  
