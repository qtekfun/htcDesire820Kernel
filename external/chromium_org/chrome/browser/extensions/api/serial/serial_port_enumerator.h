// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SERIAL_SERIAL_PORT_ENUMERATOR_H_
#define CHROME_BROWSER_EXTENSIONS_API_SERIAL_SERIAL_PORT_ENUMERATOR_H_

#include <set>
#include <string>

#include "base/gtest_prod_util.h"

FORWARD_DECLARE_TEST(SerialPortEnumeratorTest, ValidPortNamePatterns);
FORWARD_DECLARE_TEST(SerialPortEnumeratorTest, InvalidPortNamePatterns);

namespace extensions {

class APIResourceEventNotifier;

class SerialPortEnumerator {
 public:
  typedef std::set<std::string> StringSet;

  
  
  static bool DoesPortExist(const StringSet& name_set,
                            const std::string& port_name);

  static StringSet GenerateValidSerialPortNames();

 private:
  
  static StringSet GenerateValidPatterns();

  FRIEND_TEST_ALL_PREFIXES(::SerialPortEnumeratorTest, ValidPortNamePatterns);
  FRIEND_TEST_ALL_PREFIXES(::SerialPortEnumeratorTest, InvalidPortNamePatterns);
};

}  

#endif  
