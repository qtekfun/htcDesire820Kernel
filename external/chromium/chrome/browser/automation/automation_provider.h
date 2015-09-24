// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_AUTOMATION_AUTOMATION_PROVIDER_H_
#define CHROME_BROWSER_AUTOMATION_AUTOMATION_PROVIDER_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/string16.h"
#include "chrome/browser/autofill/field_types.h"
#include "chrome/common/automation_constants.h"
#include "chrome/common/content_settings.h"
#include "content/browser/browser_thread.h"
#include "content/browser/cancelable_request.h"
#include "content/browser/tab_contents/navigation_entry.h"
#include "content/common/notification_observer.h"
#include "ipc/ipc_channel.h"

#if defined(OS_WIN)
#include "ui/gfx/native_widget_types.h"
#include "views/events/event.h"
#endif  

class PopupMenuWaiter;
class TabContents;
struct AutomationMsg_Find_Params;
struct Reposition_Params;
struct ExternalTabSettings;

namespace IPC {
class ChannelProxy;
}

class AutofillProfile;
class AutomationAutocompleteEditTracker;
class AutomationBrowserTracker;
class AutomationExtensionTracker;
class AutomationResourceMessageFilter;
class AutomationTabTracker;
class AutomationWindowTracker;
class Browser;
class CreditCard;
class DictionaryValue;
class DownloadItem;
class Extension;
class ExtensionPortContainer;
class ExtensionTestResultNotificationObserver;
class ExternalTabContainer;
class FilePath;
class InitialLoadObserver;
class ListValue;
class LoginHandler;
class MetricEventDurationObserver;
class NavigationController;
class NavigationControllerRestoredObserver;
class Profile;
class RenderViewHost;
class TabContents;
struct AutocompleteMatchData;

namespace gfx {
class Point;
}

class AutomationProvider
    : public IPC::Channel::Listener,
      public IPC::Message::Sender,
      public base::SupportsWeakPtr<AutomationProvider>,
      public base::RefCountedThreadSafe<AutomationProvider,
                                        BrowserThread::DeleteOnUIThread> {
 public:
  explicit AutomationProvider(Profile* profile);

  Profile* profile() const { return profile_; }

  
  
  
  
  
  
  
  bool InitializeChannel(const std::string& channel_id) WARN_UNUSED_RESULT;

  
  
  void SetExpectedTabCount(size_t expected_tabs);

  
  
  void OnInitialTabLoadsComplete();

  
  void OnNetworkLibraryInit();

  
  
  
  int GetIndexForNavigationController(const NavigationController* controller,
                                      const Browser* parent) const;

  
  
  
  
  
  
  void AddLoginHandler(NavigationController* tab, LoginHandler* handler);
  void RemoveLoginHandler(NavigationController* tab);

  
  virtual bool Send(IPC::Message* msg);
  virtual void OnChannelConnected(int pid);
  virtual bool OnMessageReceived(const IPC::Message& msg);
  virtual void OnChannelError();

  IPC::Message* reply_message_release() {
    IPC::Message* reply_message = reply_message_;
    reply_message_ = NULL;
    return reply_message;
  }

  
  
  
  int AddExtension(const Extension* extension);

#if defined(OS_WIN)
  
  bool AddExternalTab(ExternalTabContainer* external_tab);
#endif

  
  
  DictionaryValue* GetDictionaryFromDownloadItem(const DownloadItem* download);

 protected:
  friend struct BrowserThread::DeleteOnThread<BrowserThread::UI>;
  friend class DeleteTask<AutomationProvider>;
  virtual ~AutomationProvider();

  
  
  
  Browser* FindAndActivateTab(NavigationController* contents);

  
  
  
  TabContents* GetTabContentsForHandle(int handle, NavigationController** tab);

  
  virtual std::string GetProtocolVersion();

  
  
  RenderViewHost* GetViewForTab(int tab_handle);

  
  
  void OnMessageDeserializationFailure();

  scoped_ptr<AutomationAutocompleteEditTracker> autocomplete_edit_tracker_;
  scoped_ptr<AutomationBrowserTracker> browser_tracker_;
  scoped_ptr<InitialLoadObserver> initial_load_observer_;
  scoped_ptr<MetricEventDurationObserver> metric_event_duration_observer_;
  scoped_ptr<NavigationControllerRestoredObserver> restore_tracker_;
  scoped_ptr<AutomationTabTracker> tab_tracker_;
  scoped_ptr<AutomationWindowTracker> window_tracker_;

  typedef std::map<NavigationController*, LoginHandler*> LoginHandlerMap;
  LoginHandlerMap login_handler_map_;

  Profile* profile_;

  
  
  
  IPC::Message* reply_message_;

  
  CancelableRequestConsumer consumer_;

  
  void SendFindRequest(
      TabContents* tab_contents,
      bool with_json,
      const string16& search_string,
      bool forward,
      bool match_case,
      bool find_next,
      IPC::Message* reply_message);

  scoped_refptr<AutomationResourceMessageFilter>
      automation_resource_message_filter_;

  
  bool reinitialize_on_channel_error_;

 private:
  void OnUnhandledMessage();

  
  bool ReinitializeChannel();

  
  void WindowSimulateDrag(int handle,
                          const std::vector<gfx::Point>& drag_path,
                          int flags,
                          bool press_escape_en_route,
                          IPC::Message* reply_message);
  void HandleUnused(const IPC::Message& message, int handle);
  void SetFilteredInet(const IPC::Message& message, bool enabled);
  void GetFilteredInetHitCount(int* hit_count);
  void SetProxyConfig(const std::string& new_proxy_config);

  
  
  void HandleFindRequest(int handle,
                         const AutomationMsg_Find_Params& params,
                         IPC::Message* reply_message);

  void OnSetPageFontSize(int tab_handle, int font_size);

  
  void RemoveBrowsingData(int remove_mask);

  
  
  
  
  void JavaScriptStressTestControl(int handle, int cmd, int param);

  void InstallExtension(const FilePath& crx_path,
                        IPC::Message* reply_message);

  void WaitForExtensionTestResult(IPC::Message* reply_message);

  void InstallExtensionAndGetHandle(const FilePath& crx_path,
                                    bool with_ui,
                                    IPC::Message* reply_message);

  void UninstallExtension(int extension_handle,
                          bool* success);

  void ReloadExtension(int extension_handle,
                       IPC::Message* reply_message);

  void EnableExtension(int extension_handle,
                       IPC::Message* reply_message);

  void DisableExtension(int extension_handle,
                        bool* success);

  void ExecuteExtensionActionInActiveTabAsync(int extension_handle,
                                              int browser_handle,
                                              IPC::Message* reply_message);

  void MoveExtensionBrowserAction(int extension_handle, int index,
                                  bool* success);

  void GetExtensionProperty(int extension_handle,
                            AutomationMsg_ExtensionProperty type,
                            bool* success,
                            std::string* value);

  
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

  
  
  const Extension* GetExtension(int extension_handle);

  
  
  const Extension* GetEnabledExtension(int extension_handle);

  
  
  const Extension* GetDisabledExtension(int extension_handle);

  
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
                         gfx::NativeWindow* tab_container_window,
                         gfx::NativeWindow* tab_window,
                         int* tab_handle,
                         int* session_id);

  void ConnectExternalTab(uint64 cookie,
                          bool allow,
                          gfx::NativeWindow parent_window,
                          gfx::NativeWindow* tab_container_window,
                          gfx::NativeWindow* tab_window,
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
  scoped_ptr<NotificationObserver> new_tab_ui_load_observer_;
  scoped_ptr<NotificationObserver> find_in_page_observer_;
  scoped_ptr<ExtensionTestResultNotificationObserver>
      extension_test_result_observer_;
  scoped_ptr<AutomationExtensionTracker> extension_tracker_;

  
  bool is_connected_;

  
  bool initial_tab_loads_complete_;

  
  bool network_library_initialized_;

  
  std::string channel_id_;

  DISALLOW_COPY_AND_ASSIGN(AutomationProvider);
};

#endif  
