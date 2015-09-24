// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_STATE_IMPL_H_
#define CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_STATE_IMPL_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/singleton.h"
#include "content/common/view_message_enums.h"
#include "content/public/browser/browser_accessibility_state.h"

namespace content {

class CONTENT_EXPORT BrowserAccessibilityStateImpl
    : public base::RefCountedThreadSafe<BrowserAccessibilityStateImpl>,
      public BrowserAccessibilityState {
 public:
  BrowserAccessibilityStateImpl();

  static BrowserAccessibilityStateImpl* GetInstance();

  virtual void EnableAccessibility() OVERRIDE;
  virtual void DisableAccessibility() OVERRIDE;
  virtual void OnScreenReaderDetected() OVERRIDE;
  virtual bool IsAccessibleBrowser() OVERRIDE;
  virtual void AddHistogramCallback(base::Closure callback) OVERRIDE;

  virtual void UpdateHistogramsForTesting() OVERRIDE;

  AccessibilityMode accessibility_mode() const { return accessibility_mode_; };
  void SetAccessibilityMode(AccessibilityMode mode);

 private:
  friend class base::RefCountedThreadSafe<BrowserAccessibilityStateImpl>;
  friend struct DefaultSingletonTraits<BrowserAccessibilityStateImpl>;

  
  
  void UpdateHistograms();

  
  virtual ~BrowserAccessibilityStateImpl();

  void UpdatePlatformSpecificHistograms();

  AccessibilityMode accessibility_mode_;

  std::vector<base::Closure> histogram_callbacks_;

  DISALLOW_COPY_AND_ASSIGN(BrowserAccessibilityStateImpl);
};

}  

#endif  
