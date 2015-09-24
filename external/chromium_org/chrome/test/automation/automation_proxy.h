// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_AUTOMATION_AUTOMATION_PROXY_H_
#define CHROME_TEST_AUTOMATION_AUTOMATION_PROXY_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/platform_thread.h"
#include "base/threading/thread.h"
#include "base/time/time.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/common/automation_constants.h"
#include "chrome/test/automation/automation_handle_tracker.h"
#include "chrome/test/automation/browser_proxy.h"
#include "ipc/ipc_channel_proxy.h"
#include "ipc/ipc_sender.h"
#include "ipc/ipc_sync_channel.h"
#include "ui/base/ui_base_types.h"
#include "ui/gfx/native_widget_types.h"
#include "url/gurl.h"

class BrowserProxy;
class TabProxy;
class WindowProxy;
struct ExternalTabSettings;

class AutomationMessageSender : public IPC::Sender {
 public:
  
  
  
  
  
  
  
  
  
  
  virtual bool Send(IPC::Message* message) = 0;
  virtual bool Send(IPC::Message* message, int timeout_ms) = 0;
};

class AutomationProxy : public IPC::Listener, public AutomationMessageSender {
 public:
  AutomationProxy(base::TimeDelta action_timeout, bool disconnect_on_failure);
  virtual ~AutomationProxy();

  
  static std::string GenerateChannelID();

  
  
  
  
  
  void InitializeChannel(const std::string& channel_id,
                         bool use_named_interface);

  
  virtual bool OnMessageReceived(const IPC::Message& msg);
  virtual void OnChannelError();

  
  void Disconnect();

  
  
  
  
  
  
  
  
  AutomationLaunchResult WaitForAppLaunch();

  
  bool WaitForProcessLauncherThreadToGoIdle() WARN_UNUSED_RESULT;

  
  
  
  bool WaitForInitialLoads() WARN_UNUSED_RESULT;

  
  
  
  
  bool WaitForInitialNewTabUILoad(int* load_time) WARN_UNUSED_RESULT;

  
  
  bool OpenNewBrowserWindow(Browser::Type type, bool show) WARN_UNUSED_RESULT;

  
  
  bool GetBrowserWindowCount(int* num_windows) WARN_UNUSED_RESULT;

  
  
  bool WaitForWindowCountToBecome(int target_count) WARN_UNUSED_RESULT;

  
  
  
  bool GetNormalBrowserWindowCount(int* num_windows) WARN_UNUSED_RESULT;

  
  bool IsURLDisplayed(GURL url) WARN_UNUSED_RESULT;

  
  
  bool GetMetricEventDuration(const std::string& event_name,
                              int* duration_ms) WARN_UNUSED_RESULT;

  
  
  
  
  
  
  scoped_refptr<BrowserProxy> GetBrowserWindow(int window_index);

  
  
  bool SendProxyConfig(const std::string& new_proxy_config) WARN_UNUSED_RESULT;

  
  
  
  void SignalAppLaunch(const std::string& version_string);
  void SignalInitialLoads();
  
  void SignalNewTabUITab(int load_time);

  
  
  bool GetExtensionTestResult(bool* result, std::string* message);

  
  bool SendJSONRequest(const std::string& request,
                       int timeout_ms,
                       std::string* response) WARN_UNUSED_RESULT;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool BeginTracing(const std::string& category_patterns) WARN_UNUSED_RESULT;

  
  bool EndTracing(std::string* json_trace_output) WARN_UNUSED_RESULT;

  IPC::SyncChannel* channel();

  
  virtual bool Send(IPC::Message* message) WARN_UNUSED_RESULT;
  virtual bool Send(IPC::Message* message, int timeout_ms) WARN_UNUSED_RESULT;

  
  
  
  virtual void InvalidateHandle(const IPC::Message& message);

  base::TimeDelta action_timeout() const {
    return action_timeout_;
  }

  
  void set_action_timeout(base::TimeDelta timeout) {
    DCHECK(timeout <= base::TimeDelta::FromMinutes(10))
        << "10+ min of automation timeout "
           "can make the test hang and be killed by buildbot";
    action_timeout_ = timeout;
  }

  
  
  
  std::string server_version() const {
    return server_version_;
  }

  
  
  
  void set_perform_version_check(bool perform_version_check) {
    perform_version_check_ = perform_version_check;
  }

  
  void SetChannel(IPC::Channel* channel);
  void ResetChannel();

  
  bool channel_disconnected_on_failure() const {
    return channel_disconnected_on_failure_;
  }

 protected:
  template <class T> scoped_refptr<T> ProxyObjectFromHandle(int handle);
  void InitializeThread();
  void InitializeHandleTracker();

  scoped_ptr<base::Thread> thread_;
  scoped_ptr<IPC::SyncChannel> channel_;
  scoped_ptr<AutomationHandleTracker> tracker_;

  base::WaitableEvent app_launched_;
  base::WaitableEvent initial_loads_complete_;
  base::WaitableEvent new_tab_ui_load_complete_;
  int new_tab_ui_load_time_;

  
  scoped_ptr<base::WaitableEvent> shutdown_event_;

  
  std::string server_version_;

  
  
  
  
  bool perform_version_check_;

  
  
  bool disconnect_on_failure_;

  
  bool channel_disconnected_on_failure_;

  
  base::TimeDelta action_timeout_;

  base::PlatformThreadId listener_thread_id_;

  DISALLOW_COPY_AND_ASSIGN(AutomationProxy);
};

#endif  
