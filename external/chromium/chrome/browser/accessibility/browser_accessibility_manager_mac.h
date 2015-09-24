// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MANAGER_MAC_H_
#define CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MANAGER_MAC_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "chrome/browser/accessibility/browser_accessibility_manager.h"

class BrowserAccessibilityManagerMac : public BrowserAccessibilityManager {
 public:
  
  virtual void NotifyAccessibilityEvent(int type, BrowserAccessibility* node);

 private:
  
  
  friend class BrowserAccessibilityManager;

  BrowserAccessibilityManagerMac(gfx::NativeView parent_view,
                                 const webkit_glue::WebAccessibility& src,
                                 BrowserAccessibilityDelegate* delegate,
                                 BrowserAccessibilityFactory* factory);

  DISALLOW_COPY_AND_ASSIGN(BrowserAccessibilityManagerMac);
};

#endif  
