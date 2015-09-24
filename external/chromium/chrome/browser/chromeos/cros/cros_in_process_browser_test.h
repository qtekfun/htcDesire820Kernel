// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_CROS_IN_PROCESS_BROWSER_TEST_H_
#define CHROME_BROWSER_CHROMEOS_CROS_CROS_IN_PROCESS_BROWSER_TEST_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/cros/cros_mock.h"
#include "chrome/test/in_process_browser_test.h"

namespace chromeos {

class CrosInProcessBrowserTest : public InProcessBrowserTest {
 public:
  CrosInProcessBrowserTest();
  virtual ~CrosInProcessBrowserTest();

 protected:
  scoped_ptr<CrosMock> cros_mock_;

  
  virtual void TearDownInProcessBrowserTestFixture();

 private:
  DISALLOW_COPY_AND_ASSIGN(CrosInProcessBrowserTest);
};

}  

#endif  
