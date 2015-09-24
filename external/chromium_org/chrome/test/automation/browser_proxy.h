// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_AUTOMATION_BROWSER_PROXY_H_
#define CHROME_TEST_AUTOMATION_BROWSER_PROXY_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/common/content_settings.h"
#include "chrome/test/automation/automation_handle_tracker.h"

class GURL;
class TabProxy;
class WindowProxy;

namespace gfx {
  class Point;
  class Rect;
}

class BrowserProxy : public AutomationResourceProxy {
 public:
  BrowserProxy(AutomationMessageSender* sender,
               AutomationHandleTracker* tracker,
               int handle)
    : AutomationResourceProxy(tracker, sender, handle) {}

  
  
  bool ActivateTab(int tab_index) WARN_UNUSED_RESULT;

  
  
  bool BringToFront() WARN_UNUSED_RESULT;

  
  
  
  bool AppendTab(const GURL& tab_url) WARN_UNUSED_RESULT;

  
  
  bool GetActiveTabIndex(int* active_tab_index) const WARN_UNUSED_RESULT;

  
  
  bool GetTabCount(int* num_tabs) const WARN_UNUSED_RESULT;

  
  
  bool GetType(Browser::Type* type) const WARN_UNUSED_RESULT;

  
  
  
  
  
  scoped_refptr<TabProxy> GetTab(int tab_index) const;

  
  
  scoped_refptr<TabProxy> GetActiveTab() const;

  
  
  
  
  scoped_refptr<WindowProxy> GetWindow() const;

  
  
  
  
  
  
  
  
  
  
  bool ApplyAccelerator(int id) WARN_UNUSED_RESULT;

  
  
  bool WaitForTabCountToBecome(int count) WARN_UNUSED_RESULT;

  
  
  
  bool WaitForTabToBecomeActive(
      int tab,
      base::TimeDelta wait_timeout) WARN_UNUSED_RESULT;

  
  
  bool IsFindWindowFullyVisible(bool* is_visible) WARN_UNUSED_RESULT;

  
  
  
  
  bool RunCommandAsync(int browser_command) const WARN_UNUSED_RESULT;

  
  
  
  
  bool RunCommand(int browser_command) const WARN_UNUSED_RESULT;

  
  
  bool TerminateSession() WARN_UNUSED_RESULT;

  
  bool SendJSONRequest(const std::string& request,
                       int timeout_ms,
                       std::string* response) WARN_UNUSED_RESULT;

  
  
  
  
  
  bool GetInitialLoadTimes(
      base::TimeDelta timeout,
      float* min_start_time,
      float* max_stop_time,
      std::vector<float>* stop_times);


 protected:
  virtual ~BrowserProxy() {}
 private:
  DISALLOW_COPY_AND_ASSIGN(BrowserProxy);
};

#endif  
