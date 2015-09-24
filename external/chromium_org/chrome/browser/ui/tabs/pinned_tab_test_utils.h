// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TABS_PINNED_TAB_TEST_UTILS_H_
#define CHROME_BROWSER_UI_TABS_PINNED_TAB_TEST_UTILS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/ui/startup/startup_tab.h"

class PinnedTabTestUtils {
 public:
  
  
  
  static std::string TabsToString(const std::vector<StartupTab>& values);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(PinnedTabTestUtils);
};

#endif  
