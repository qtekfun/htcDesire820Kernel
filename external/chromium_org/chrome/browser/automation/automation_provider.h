// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_AUTOMATION_AUTOMATION_PROVIDER_H_
#define CHROME_BROWSER_AUTOMATION_AUTOMATION_PROVIDER_H_

#include <list>
#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/sequenced_task_runner_helpers.h"
#include "base/strings/string16.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/common/automation_constants.h"
#include "chrome/common/content_settings.h"
#include "components/autofill/core/browser/field_types.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/notification_observer.h"
#include "ipc/ipc_channel.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"

#if defined(OS_WIN) && !defined(USE_AURA)
#include "ui/gfx/native_widget_types.h"
#endif  

class AutomationBrowserTracker;
class AutomationResourceMessageFilter;
class AutomationTabTracker;
class AutomationWindowTracker;
class Browser;
class ExternalTabContainer;
class FindInPageNotificationObserver;
class InitialLoadObserver;
class LoginHandler;
class MetricEventDurationObserver;
class NavigationControllerRestoredObserver;
class NewTabUILoadObserver;
class Profile;
struct AutomationMsg_Find_Params;
struct Reposition_Params;
struct ExternalTabSettings;

namespace IPC {
class ChannelProxy;
}

namespace content {
class NavigationController;
class RenderViewHost;
}

namespace base {
class DictionaryValue;
}

namespace content {
class DownloadItem;
class WebContents;
}

namespace gfx {
class Point;
}

class AutomationProvider
    : public IPC::Listener,
      public IPC::Sender,
      public base::SupportsWeakPtr<AutomationProvider>,
      public base::RefCountedThreadSafe<
          AutomationProvider, content::BrowserThread::DeleteOnUIThread> {
 public:
  explicit AutomationProvider(Profile* profile);

  Profile* profile() const { return profile_; }

  void set_profile(Profile* profile);

  
  
  
  
  
  
  
  bool InitializeChannel(const std::string& channel_id) WARN_UNUSED_RESULT;

  virtual IPC::Channel::Mode GetChannelMode(bool use_named_interface);

  
  
  void SetExpectedTabCount(size_t expected_tabs);

  
  
  void OnInitialTabLoadsComplete();

  
  void OnOOBEWebuiReady();

  
  
  void SendInitialLoadMessage();

  
  
  
  void DisableInitialLoadObservers();

  
  
  
  int GetIndexForNavigationController(
      const content::NavigationController* controller,
      const Browser* parent) const;

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  virtual void OnChannelConnected(int pid) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  IPC::Message* reply_message_release() {
    IPC::Message* reply_message = reply_message_;
    reply_message_ = NULL;
    return reply_message;
  }

#if defined(OS_WIN)
  
  bool AddExternalTab(ExternalTabContainer* external_tab);
#endif

  
  
  base::DictionaryValue* GetDictionaryFromDownloadItem(
      const content::DownloadItem* download,
      bool incognito);

 protected:
  friend struct content::BrowserThread::DeleteOnThread<
      content::BrowserThread::UI>;
  friend class base::DeleteHelper<AutomationProvider>;
  virtual ~AutomationProvider();

  
  
  
  Browser* FindAndActivateTab(content::NavigationController* contents);

  
  
  
  content::WebContents* GetWebContentsForHandle(
      int handle, content::NavigationController** tab);

  
  virtual std::string GetProtocolVersion();

  
  
  content::RenderViewHost* GetViewForTab(int tab_handle);

  
  
  void OnMessageDeserializationFailure();

  scoped_ptr<AutomationBrowserTracker> browser_tracker_;
  scoped_ptr<InitialLoadObserver> initial_load_observer_;
  scoped_ptr<MetricEventDurationObserver> metric_event_duration_observer_;
  scoped_ptr<AutomationTabTracker> tab_tracker_;
  scoped_ptr<AutomationWindowTracker> window_tracker_;

  Profile* profile_;

  
  
  
  IPC::Message* reply_message_;

  
  CancelableRequestConsumer consumer_;

  
  void SendFindRequest(
      content::WebContents* web_contents,
      bool with_json,
      const base::string16& search_string,
      bool forward,
      bool match_case,
      bool find_next,
      IPC::Message* reply_message);

  scoped_refptr<AutomationResourceMessageFilter>
      automation_resource_message_filter_;

  
  bool reinitialize_on_channel_error_;

 private:
  void OnUnhandledMessage(const IPC::Message& message);

  
  bool ReinitializeChannel();

  void HandleUnused(const IPC::Message& message, int handle);
  void GetFilteredInetHitCount(int* hit_count);
  void SetProxyConfig(const std::string& new_proxy_config);

  
  
  void HandleFindRequest(int handle,
                         const AutomationMsg_Find_Params& params,
                         IPC::Message* reply_message);

  void OnSetPageFontSize(int tab_handle, int font_size);

  
  void RemoveBrowsingData(int remove_mask);

  
  
  
  
  void JavaScriptStressTestControl(int handle, int cmd, int param);

  void BeginTracing(const std::string& category_patterns, bool* success);
  void EndTracing(IPC::Message* reply_message);
  void OnTraceDataCollected(IPC::Message* reply_message,
                            const base::FilePath& path);

  
  void PrintAsync(int tab_handle);

  
  
  void OverrideEncoding(int tab_handle,
                        const std::string& encoding_name,
                        bool* success);

  
  void SelectAll(int tab_handle);

  
  void Cut(int tab_handle);
  void Copy(int tab_handle);
  void Paste(int tab_handle);

  void ReloadAsync(int tab_handle);
  void StopAsync(int tab_handle);
  void SaveAsAsync(int tab_handle);

  
  void NotifyPopupMenuOpened();

#if defined(OS_WIN)
  
  

  
  
  
  void ProcessUnhandledAccelerator(const IPC::Message& message, int handle,
                                   const MSG& msg);

  void SetInitialFocus(const IPC::Message& message, int handle, bool reverse,
                       bool restore_focus_to_view);

  void OnTabReposition(int tab_handle,
                       const Reposition_Params& params);

  void OnForwardContextMenuCommandToChrome(int tab_handle, int command);

  void CreateExternalTab(const ExternalTabSettings& settings,
                         HWND* tab_container_window,
                         HWND* tab_window,
                         int* tab_handle,
                         int* session_id);

  void ConnectExternalTab(uint64 cookie,
                          bool allow,
                          HWND parent_window,
                          HWND* tab_container_window,
                          HWND* tab_window,
                          int* tab_handle,
                          int* session_id);

  void NavigateInExternalTab(
      int handle, const GURL& url, const GURL& referrer,
      AutomationMsg_NavigationResponseValues* status);
  void NavigateExternalTabAtIndex(
      int handle, int index, AutomationMsg_NavigationResponseValues* status);

  
  void OnMessageFromExternalHost(int handle, const std::string& message,
                                 const std::string& origin,
                                 const std::string& target);

  void OnBrowserMoved(int handle);

  void OnRunUnloadHandlers(int handle, IPC::Message* reply_message);

  void OnSetZoomLevel(int handle, int zoom_level);

  ExternalTabContainer* GetExternalTabForHandle(int handle);
#endif  

  scoped_ptr<IPC::ChannelProxy> channel_;
  scoped_ptr<NewTabUILoadObserver> new_tab_ui_load_observer_;
  scoped_ptr<FindInPageNotificationObserver> find_in_page_observer_;

  
  bool use_initial_load_observers_;

  
  bool is_connected_;

  
  bool initial_tab_loads_complete_;

  
  bool login_webui_ready_;

  
  std::string channel_id_;

  DISALLOW_COPY_AND_ASSIGN(AutomationProvider);
};

#endif  
