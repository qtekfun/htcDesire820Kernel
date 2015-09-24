// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TABS_PINNED_TAB_TEST_UTILS_H_
#define CHROME_BROWSER_TABS_PINNED_TAB_TEST_UTILS_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/ui/browser_init.h"

class PinnedTabTestUtils {
 public:
  
  
  
  static std::string TabsToString(
      const std::vector<BrowserInit::LaunchWithProfile::Tab>& values);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(PinnedTabTestUtils);
};

#endif  
