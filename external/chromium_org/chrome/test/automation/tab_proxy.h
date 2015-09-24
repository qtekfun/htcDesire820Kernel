// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_AUTOMATION_TAB_PROXY_H_
#define CHROME_TEST_AUTOMATION_TAB_PROXY_H_

#include "build/build_config.h"  

#if defined(OS_WIN)
#include <wtypes.h>  
#endif

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "chrome/common/automation_constants.h"
#include "chrome/test/automation/automation_handle_tracker.h"
#include "content/public/browser/save_page_type.h"
#include "content/public/common/page_type.h"
#include "content/public/common/security_style.h"
#include "net/cert/cert_status_flags.h"
#include "ui/base/window_open_disposition.h"
#include "ui/events/keycodes/keyboard_codes.h"

class BrowserProxy;
class GURL;

namespace IPC {
class Message;
}

namespace base {
class FilePath;
class Value;
}

class TabProxy : public AutomationResourceProxy {
 public:
  class TabProxyDelegate {
   public:
    virtual bool OnMessageReceived(TabProxy* tab, const IPC::Message& msg) {
      return false;
    }
    virtual void OnChannelError(TabProxy* tab) {}

   protected:
    virtual ~TabProxyDelegate() {}
  };

  TabProxy(AutomationMessageSender* sender,
           AutomationHandleTracker* tracker,
           int handle);

  
  bool GetCurrentURL(GURL* url) const WARN_UNUSED_RESULT;

  
  bool GetTabTitle(std::wstring* title) const WARN_UNUSED_RESULT;

  
  bool GetTabIndex(int* index) const WARN_UNUSED_RESULT;

  
  
  
  
  
  
  
  
  
  bool ExecuteAndExtractString(const std::wstring& frame_xpath,
                               const std::wstring& jscript,
                               std::wstring* value) WARN_UNUSED_RESULT;
  bool ExecuteAndExtractBool(const std::wstring& frame_xpath,
                             const std::wstring& jscript,
                             bool* value) WARN_UNUSED_RESULT;
  bool ExecuteAndExtractInt(const std::wstring& frame_xpath,
                            const std::wstring& jscript,
                            int* value) WARN_UNUSED_RESULT;

  
  
  
  AutomationMsg_NavigationResponseValues NavigateToURL(
      const GURL& url) WARN_UNUSED_RESULT;

  
  
  
  AutomationMsg_NavigationResponseValues
      NavigateToURLBlockUntilNavigationsComplete(
          const GURL& url, int number_of_navigations) WARN_UNUSED_RESULT;


  
  
  
  
  
  
  bool NavigateToURLAsync(const GURL& url) WARN_UNUSED_RESULT;

  
  
  AutomationMsg_NavigationResponseValues GoBack() WARN_UNUSED_RESULT;

  
  
  AutomationMsg_NavigationResponseValues GoBackBlockUntilNavigationsComplete(
      int number_of_navigations) WARN_UNUSED_RESULT;

  
  
  AutomationMsg_NavigationResponseValues GoForward() WARN_UNUSED_RESULT;

  
  
  AutomationMsg_NavigationResponseValues GoForwardBlockUntilNavigationsComplete(
      int number_of_navigations) WARN_UNUSED_RESULT;

  
  
  AutomationMsg_NavigationResponseValues Reload() WARN_UNUSED_RESULT;

  
  
  
  
  
  bool Close() WARN_UNUSED_RESULT;

  
  
  
  
  
  
  
  
  
  
  bool Close(bool wait_until_closed) WARN_UNUSED_RESULT;

  
  
  
  
  
  
  
  int FindInPage(const std::wstring& search_string, FindInPageDirection forward,
                 FindInPageCase match_case, bool find_next, int* ordinal);

  bool GetCookies(const GURL& url, std::string* cookies) WARN_UNUSED_RESULT;
  bool GetCookieByName(const GURL& url,
                       const std::string& name,
                       std::string* cookies) WARN_UNUSED_RESULT;

#if defined(OS_WIN)
  

  
  
  
  bool ProcessUnhandledAccelerator(const MSG& msg) WARN_UNUSED_RESULT;

  
  
  
  
  bool SetInitialFocus(bool reverse, bool restore_focus_to_view)
      WARN_UNUSED_RESULT;

  
  
  
  
  AutomationMsg_NavigationResponseValues NavigateInExternalTab(
      const GURL& url, const GURL& referrer) WARN_UNUSED_RESULT;

  AutomationMsg_NavigationResponseValues NavigateExternalTabAtIndex(
      int index) WARN_UNUSED_RESULT;

  
  void HandleMessageFromExternalHost(const std::string& message,
                                     const std::string& origin,
                                     const std::string& target);
#endif  

  
  bool PrintAsync() WARN_UNUSED_RESULT;

  
  
  bool WaitForInfoBarCount(size_t count) WARN_UNUSED_RESULT;

  
  bool OverrideEncoding(const std::string& encoding) WARN_UNUSED_RESULT;

#if defined(OS_WIN)
  
  
  
  void Reposition(HWND window, HWND window_insert_after, int left, int top,
                  int width, int height, int flags, HWND parent_window);

  
  void SendContextMenuCommand(int selected_command);

  
  void OnHostMoved();
#endif  

  
  void SelectAll();

  
  void Cut();
  void Copy();
  void Paste();

  
  
  void ReloadAsync();
  void StopAsync();
  void SaveAsAsync();

  
  
  
  
  void JavaScriptStressTestControl(int cmd, int param);

  
  void AddObserver(TabProxyDelegate* observer);
  void RemoveObserver(TabProxyDelegate* observer);
  bool OnMessageReceived(const IPC::Message& message);
  void OnChannelError();
 protected:
  virtual ~TabProxy();

  
  
  void FirstObjectAdded();

  
  
  void LastObjectRemoved();

  
  base::Value* ExecuteAndExtractValue(
      const std::wstring& frame_xpath,
      const std::wstring& jscript) WARN_UNUSED_RESULT;

 private:
  base::Lock list_lock_;  
  ObserverList<TabProxyDelegate> observers_list_;
  DISALLOW_COPY_AND_ASSIGN(TabProxy);
};

#endif  
