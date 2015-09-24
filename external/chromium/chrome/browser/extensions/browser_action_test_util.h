// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_BROWSER_ACTION_TEST_UTIL_H_
#define CHROME_BROWSER_EXTENSIONS_BROWSER_ACTION_TEST_UTIL_H_
#pragma once

#include <string>

#include "build/build_config.h"

class Browser;

namespace gfx {
class Rect;
class Size;
}  

class BrowserActionTestUtil {
 public:
  explicit BrowserActionTestUtil(Browser* browser) : browser_(browser) {}

  
  int NumberOfBrowserActions();

  
  int VisibleBrowserActions();

#if defined(TOOLKIT_VIEWS)
  
  void WaitForBrowserActionUpdated(int index);
#endif

  
  
  
  bool HasIcon(int index);

  
  void Press(int index);

  
  std::string GetExtensionId(int index);

  
  std::string GetTooltip(int index);

  
  bool HasPopup();

  
  gfx::Rect GetPopupBounds();

  
  bool HidePopup();

  
  void SetIconVisibilityCount(size_t icons);

  
  static gfx::Size GetMinPopupSize();

  
  static gfx::Size GetMaxPopupSize();

 private:
  Browser* browser_;  
};

#endif  
