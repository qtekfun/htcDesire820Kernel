// Copyright 2008 Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GTEST_TEST_GTEST_TYPED_TEST_TEST_H_
#define GTEST_TEST_GTEST_TYPED_TEST_TEST_H_

#include "gtest/gtest.h"

#if GTEST_HAS_TYPED_TEST_P

using testing::Test;


template <typename T>
class ContainerTest : public Test {
};

TYPED_TEST_CASE_P(ContainerTest);

TYPED_TEST_P(ContainerTest, CanBeDefaultConstructed) {
  TypeParam container;
}

TYPED_TEST_P(ContainerTest, InitialSizeIsZero) {
  TypeParam container;
  EXPECT_EQ(0U, container.size());
}

REGISTER_TYPED_TEST_CASE_P(ContainerTest,
                           CanBeDefaultConstructed, InitialSizeIsZero);

#endif  

#endif  
