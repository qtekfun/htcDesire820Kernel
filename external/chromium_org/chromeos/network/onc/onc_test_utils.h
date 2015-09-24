// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_ONC_ONC_TEST_UTILS_H_
#define CHROMEOS_NETWORK_ONC_ONC_TEST_UTILS_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {
class DictionaryValue;
class Value;
}

namespace chromeos {
namespace onc {
namespace test_utils {

std::string ReadTestData(const std::string& filename);

scoped_ptr<base::DictionaryValue> ReadTestDictionary(
    const std::string& filename);

::testing::AssertionResult Equals(const base::Value* expected,
                                  const base::Value* actual);

}  
}  
}  

#endif  
