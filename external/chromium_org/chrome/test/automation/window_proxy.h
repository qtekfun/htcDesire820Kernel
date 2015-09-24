// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_AUTOMATION_WINDOW_PROXY_H__
#define CHROME_TEST_AUTOMATION_WINDOW_PROXY_H__

#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#endif

#include "base/strings/string16.h"
#include "base/threading/thread.h"
#include "chrome/test/automation/automation_handle_tracker.h"

class BrowserProxy;
class WindowProxy;

namespace gfx {
class Rect;
}

class WindowProxy : public AutomationResourceProxy {
 public:
  WindowProxy(AutomationMessageSender* sender,
              AutomationHandleTracker* tracker,
              int handle)
    : AutomationResourceProxy(tracker, sender, handle) {}

  
  
  
  
  bool GetViewBounds(int view_id, gfx::Rect* bounds, bool screen_coordinates);

  
  
  bool SetBounds(const gfx::Rect& bounds);

 protected:
  virtual ~WindowProxy() {}
 private:
  DISALLOW_COPY_AND_ASSIGN(WindowProxy);
};

#endif  
