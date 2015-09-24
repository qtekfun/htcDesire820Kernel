// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MANAGER_WIN_H_
#define CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MANAGER_WIN_H_
#pragma once

#include <oleacc.h>

#include "base/win/scoped_comptr.h"
#include "chrome/browser/accessibility/browser_accessibility_manager.h"
#include "webkit/glue/webaccessibility.h"

class BrowserAccessibilityWin;
struct ViewHostMsg_AccessibilityNotification_Params;

using webkit_glue::WebAccessibility;

class BrowserAccessibilityManagerWin : public BrowserAccessibilityManager {
 public:
  virtual ~BrowserAccessibilityManagerWin();

  
  
  IAccessible* GetParentWindowIAccessible();

  
  virtual void NotifyAccessibilityEvent(int type, BrowserAccessibility* node);

 private:
  BrowserAccessibilityManagerWin(
      HWND parent_window,
      const WebAccessibility& src,
      BrowserAccessibilityDelegate* delegate,
      BrowserAccessibilityFactory* factory);

  
  base::win::ScopedComPtr<IAccessible> window_iaccessible_;

  
  friend class BrowserAccessibilityManager;

  DISALLOW_COPY_AND_ASSIGN(BrowserAccessibilityManagerWin);
};

#endif  
