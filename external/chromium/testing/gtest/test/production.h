// Copyright 2006, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GTEST_TEST_PRODUCTION_H_
#define GTEST_TEST_PRODUCTION_H_

#include "gtest/gtest_prod.h"

class PrivateCode {
 public:
  
  FRIEND_TEST(PrivateCodeTest, CanAccessPrivateMembers);

  
  FRIEND_TEST(PrivateCodeFixtureTest, CanAccessPrivateMembers);

  PrivateCode();

  int x() const { return x_; }
 private:
  void set_x(int an_x) { x_ = an_x; }
  int x_;
};

#endif  
