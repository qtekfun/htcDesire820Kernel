// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_CHROME_UNIT_TEST_SUITE_H_
#define CHROME_TEST_BASE_CHROME_UNIT_TEST_SUITE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/test/base/chrome_test_suite.h"

class ChromeUnitTestSuite : public ChromeTestSuite {
 public:
  ChromeUnitTestSuite(int argc, char** argv);
  virtual ~ChromeUnitTestSuite();

  
  virtual void Initialize() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(ChromeUnitTestSuite);
};

#endif  
