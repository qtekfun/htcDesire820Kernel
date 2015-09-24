// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MAC_H_
#define CHROME_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MAC_H_
#pragma once

#include <map>
#include <utility>
#include <vector>

#include "base/memory/scoped_nsobject.h"
#include "chrome/browser/accessibility/browser_accessibility.h"

@class BrowserAccessibilityCocoa;

class BrowserAccessibilityMac : public BrowserAccessibility {
 public:
  
  virtual void Initialize();
  virtual void NativeReleaseReference();

  
  
  virtual void ReplaceChild(
      BrowserAccessibility* old_acc,
      BrowserAccessibility* new_acc);

  
  BrowserAccessibilityCocoa* native_view() const {
    return browser_accessibility_cocoa_;
  }

 private:
  
  friend class BrowserAccessibility;

  BrowserAccessibilityMac();

  
  
  
  
  BrowserAccessibilityCocoa* browser_accessibility_cocoa_;
  DISALLOW_COPY_AND_ASSIGN(BrowserAccessibilityMac);
};

#endif 
