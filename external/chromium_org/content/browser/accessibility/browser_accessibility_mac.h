// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MAC_H_
#define CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MAC_H_

#include <map>
#include <utility>
#include <vector>

#include "content/browser/accessibility/browser_accessibility.h"

@class BrowserAccessibilityCocoa;

namespace content {

class BrowserAccessibilityMac : public BrowserAccessibility {
 public:
  
  virtual void PreInitialize() OVERRIDE;
  virtual void NativeReleaseReference() OVERRIDE;
  virtual bool IsNative() const OVERRIDE;

  
  virtual void DetachTree(std::vector<BrowserAccessibility*>* nodes) OVERRIDE;
  virtual void SwapChildren(std::vector<BrowserAccessibility*>& children)
      OVERRIDE;

  
  BrowserAccessibilityCocoa* native_view() const {
    return browser_accessibility_cocoa_;
  }

 private:
  
  friend class BrowserAccessibility;

  BrowserAccessibilityMac();

  
  
  
  
  BrowserAccessibilityCocoa* browser_accessibility_cocoa_;
  DISALLOW_COPY_AND_ASSIGN(BrowserAccessibilityMac);
};

}  

#endif 
