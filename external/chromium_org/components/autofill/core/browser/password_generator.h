// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_PASSWORD_GENERATOR_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_PASSWORD_GENERATOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"

namespace autofill {

class PasswordGenerator {
 public:
  
  explicit PasswordGenerator(size_t max_length);
  ~PasswordGenerator();

  
  
  
  
  
  
  
  
  std::string Generate() const;

 private:
  
  static const size_t kDefaultPasswordLength;
  FRIEND_TEST_ALL_PREFIXES(PasswordGeneratorTest, PasswordLength);

  
  const size_t password_length_;

  DISALLOW_COPY_AND_ASSIGN(PasswordGenerator);
};

}  

#endif  
