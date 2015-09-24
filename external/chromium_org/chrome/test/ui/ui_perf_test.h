// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_UI_UI_PERF_TEST_H_
#define CHROME_TEST_UI_UI_PERF_TEST_H_

#include "chrome/test/ui/ui_test.h"

class UIPerfTest : public UITest {
 protected:
  
  virtual void SetLaunchSwitches() OVERRIDE;

  
  void PrintIOPerfInfo(const char* test_name);

  
  void PrintMemoryUsageInfo(const char* test_name);

  
  void UseReferenceBuild();
};

#endif  
