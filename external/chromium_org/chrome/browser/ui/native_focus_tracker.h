// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_NATIVE_FOCUS_TRACKER_H_
#define CHROME_BROWSER_UI_NATIVE_FOCUS_TRACKER_H_

#include "base/basictypes.h"

class Browser;

class NativeFocusTrackerHost {
 public:
  virtual void SetBrowser(Browser* browser) = 0;

 protected:
  virtual ~NativeFocusTrackerHost() {}
};

class NativeFocusTracker {
 public:
  virtual ~NativeFocusTracker() {}

  
  static NativeFocusTracker* Create(NativeFocusTrackerHost* host);

 protected:
  NativeFocusTracker() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(NativeFocusTracker);
};

#endif  
