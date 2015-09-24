// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_BROWSER_ACCESSIBILITY_STATE_H_
#define CONTENT_PUBLIC_BROWSER_BROWSER_ACCESSIBILITY_STATE_H_

#include "base/callback_forward.h"

#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT BrowserAccessibilityState {
 public:
  virtual ~BrowserAccessibilityState() { }

  
  static BrowserAccessibilityState* GetInstance();

  
  virtual void EnableAccessibility() = 0;

  
  virtual void DisableAccessibility() = 0;

  
  virtual void OnScreenReaderDetected() = 0;

  
  virtual bool IsAccessibleBrowser() = 0;

  
  
  
  
  virtual void AddHistogramCallback(base::Closure callback) = 0;

  virtual void UpdateHistogramsForTesting() = 0;
};

}  

#endif  
