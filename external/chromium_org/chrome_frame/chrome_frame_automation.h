// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_CHROME_FRAME_AUTOMATION_H_
#define CHROME_FRAME_CHROME_FRAME_AUTOMATION_H_

#include <atlbase.h>
#include <atlwin.h>
#include <map>
#include <string>
#include <vector>

#include "base/containers/stack_container.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_handle.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread.h"
#include "base/timer/timer.h"
#include "chrome/test/automation/automation_proxy.h"
#include "chrome/test/automation/tab_proxy.h"
#include "chrome_frame/chrome_frame_delegate.h"
#include "chrome_frame/plugin_url_request.h"
#include "chrome_frame/sync_msg_reply_dispatcher.h"
#include "content/public/common/page_zoom.h"

const unsigned long kCommandExecutionTimeout = 60000;  

class ProxyFactory;
class NavigationConstraints;
enum AutomationPageFontSize;

struct DECLSPEC_NOVTABLE ChromeFrameAutomationProxy {  
  virtual bool Send(IPC::Message* msg) = 0;

  virtual void SendAsAsync(
      IPC::SyncMessage* msg,
      SyncMessageReplyDispatcher::SyncMessageCallContext* context,
      void* key) = 0;
  virtual void CancelAsync(void* key) = 0;
  virtual scoped_refptr<TabProxy> CreateTabProxy(int handle) = 0;
  virtual void ReleaseTabProxy(AutomationHandle handle) = 0;
  virtual std::string server_version() = 0;

  virtual void SendProxyConfig(const std::string&) = 0;
 protected:
  virtual ~ChromeFrameAutomationProxy() {}
};

class ProxyFactory;

class ChromeFrameAutomationProxyImpl
  : public ChromeFrameAutomationProxy,
    
    
    
    public AutomationProxy {
 public:
  ~ChromeFrameAutomationProxyImpl();
  virtual void SendAsAsync(
      IPC::SyncMessage* msg,
      SyncMessageReplyDispatcher::SyncMessageCallContext* context,
      void* key);

  
  virtual void OnChannelError();

  virtual void CancelAsync(void* key);

  virtual scoped_refptr<TabProxy> CreateTabProxy(int handle);
  virtual void ReleaseTabProxy(AutomationHandle handle);

  virtual std::string server_version() {
    return AutomationProxy::server_version();
  }

  virtual bool Send(IPC::Message* msg) {
    return AutomationProxy::Send(msg);
  }

  virtual void SendProxyConfig(const std::string& p) {
    AutomationProxy::SendProxyConfig(p);
  }

 protected:
  friend class AutomationProxyCacheEntry;
  ChromeFrameAutomationProxyImpl(AutomationProxyCacheEntry* entry,
                                 std::string channel_id,
                                 base::TimeDelta launch_timeout);

  class CFMsgDispatcher;
  class TabProxyNotificationMessageFilter;

  scoped_refptr<CFMsgDispatcher> sync_;
  scoped_refptr<TabProxyNotificationMessageFilter> message_filter_;
  AutomationProxyCacheEntry* proxy_entry_;
};

class ChromeFrameLaunchParams :  
    public base::RefCountedThreadSafe<ChromeFrameLaunchParams> {
 public:
  ChromeFrameLaunchParams(const GURL& url, const GURL& referrer,
                          const base::FilePath& profile_path,
                          const std::wstring& profile_name,
                          const std::wstring& language,
                          bool incognito, bool widget_mode,
                          bool route_all_top_level_navigations)
    : launch_timeout_(kCommandExecutionTimeout), url_(url),
      referrer_(referrer), profile_path_(profile_path),
      profile_name_(profile_name), language_(language),
      version_check_(true), incognito_mode_(incognito),
      is_widget_mode_(widget_mode),
      route_all_top_level_navigations_(route_all_top_level_navigations) {
  }

  ~ChromeFrameLaunchParams() {
  }

  void set_launch_timeout(int timeout) {
    launch_timeout_ = timeout;
  }

  int launch_timeout() const {
    return launch_timeout_;
  }

  const GURL& url() const {
    return url_;
  }

  void set_url(const GURL& url) {
    url_ = url;
  }

  const GURL& referrer() const {
    return referrer_;
  }

  void set_referrer(const GURL& referrer) {
    referrer_ = referrer;
  }

  const base::FilePath& profile_path() const {
    return profile_path_;
  }

  const std::wstring& profile_name() const {
    return profile_name_;
  }

  const std::wstring& language() const {
    return language_;
  }

  bool version_check() const {
    return version_check_;
  }

  void set_version_check(bool check) {
    version_check_ = check;
  }

  bool incognito() const {
    return incognito_mode_;
  }

  bool widget_mode() const {
    return is_widget_mode_;
  }

  void set_route_all_top_level_navigations(
      bool route_all_top_level_navigations) {
    route_all_top_level_navigations_ = route_all_top_level_navigations;
  }

  bool route_all_top_level_navigations() const {
    return route_all_top_level_navigations_;
  }

 protected:
  int launch_timeout_;
  GURL url_;
  GURL referrer_;
  base::FilePath profile_path_;
  std::wstring profile_name_;
  std::wstring language_;
  bool version_check_;
  bool incognito_mode_;
  bool is_widget_mode_;
  bool route_all_top_level_navigations_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ChromeFrameLaunchParams);
};

struct DECLSPEC_NOVTABLE LaunchDelegate {  
  virtual void LaunchComplete(ChromeFrameAutomationProxy* proxy,
                              AutomationLaunchResult result) = 0;
  virtual void AutomationServerDied() = 0;
};  

class AutomationProxyCacheEntry
    : public base::RefCounted<AutomationProxyCacheEntry> {
 public:
  AutomationProxyCacheEntry(ChromeFrameLaunchParams* params,
                            LaunchDelegate* delegate);

  ~AutomationProxyCacheEntry();

  void AddDelegate(LaunchDelegate* delegate);
  void RemoveDelegate(LaunchDelegate* delegate, base::WaitableEvent* done,
                      bool* was_last_delegate);

  DWORD WaitForThread(DWORD timeout) {  
    DCHECK(thread_.get());
    return ::WaitForSingleObject(thread_->thread_handle().platform_handle(),
                                 timeout);
  }

  bool IsSameProfile(const std::wstring& name) const {
    return lstrcmpiW(name.c_str(), profile_name.c_str()) == 0;
  }

  base::Thread* thread() const {
    return thread_.get();
  }

  base::MessageLoop* message_loop() const {
    return thread_->message_loop();
  }

  bool IsSameThread(base::PlatformThreadId id) const {
    return thread_->thread_id() == id;
  }

  ChromeFrameAutomationProxyImpl* proxy() const {
    DCHECK(IsSameThread(base::PlatformThread::CurrentId()));
    return proxy_.get();
  }

  
  void OnChannelError();

 protected:
  void CreateProxy(ChromeFrameLaunchParams* params,
                   LaunchDelegate* delegate);

 protected:
  std::wstring profile_name;
  scoped_ptr<base::Thread> thread_;
  scoped_ptr<ChromeFrameAutomationProxyImpl> proxy_;
  AutomationLaunchResult launch_result_;
  typedef std::vector<LaunchDelegate*> LaunchDelegates;
  LaunchDelegates launch_delegates_;
  
  
  base::TimeTicks automation_server_launch_start_time_;
};

class ProxyFactory {
 public:
  ProxyFactory();
  virtual ~ProxyFactory();

  
  
  
  
  virtual void GetAutomationServer(LaunchDelegate* delegate,
                                   ChromeFrameLaunchParams* params,
                                   void** automation_server_id);
  virtual bool ReleaseAutomationServer(void* server_id,
                                       LaunchDelegate* delegate);

 private:
  typedef base::StackVector<scoped_refptr<AutomationProxyCacheEntry>, 4> Vector;
  Vector proxies_;
  
  base::Lock lock_;
};

class ChromeFrameAutomationClient
    : public CWindowImpl<ChromeFrameAutomationClient>,
      public TaskMarshallerThroughWindowsMessages<ChromeFrameAutomationClient>,
      public base::RefCountedThreadSafe<ChromeFrameAutomationClient>,
      public PluginUrlRequestDelegate,
      public TabProxy::TabProxyDelegate,
      public LaunchDelegate {
 public:
  ChromeFrameAutomationClient();
  ~ChromeFrameAutomationClient();

  
  virtual bool Initialize(ChromeFrameDelegate* chrome_frame_delegate,
                          ChromeFrameLaunchParams* chrome_launch_params);
  void Uninitialize();
  void NotifyAndUninitialize();

  virtual bool InitiateNavigation(
      const std::string& url,
      const std::string& referrer,
      NavigationConstraints* navigation_constraints);

  virtual bool NavigateToIndex(int index);
  bool ForwardMessageFromExternalHost(const std::string& message,
                                      const std::string& origin,
                                      const std::string& target);
  bool SetProxySettings(const std::string& json_encoded_proxy_settings);

  void FindInPage(const std::wstring& search_string,
                  FindInPageDirection forward,
                  FindInPageCase match_case,
                  bool find_next);

  virtual void OnChromeFrameHostMoved();

  TabProxy* tab() const { return tab_.get(); }

  BEGIN_MSG_MAP(ChromeFrameAutomationClient)
    CHAIN_MSG_MAP(
        TaskMarshallerThroughWindowsMessages<ChromeFrameAutomationClient>)
  END_MSG_MAP()

  
  
  
  void Resize(int width, int height, int flags);

  
  void SetParentWindow(HWND parent_window);

  void SendContextMenuCommandToChromeFrame(int selected_command);

  HWND tab_window() const {
    return tab_window_;
  }

  void ReleaseAutomationServer();

  
  std::wstring GetVersion() const;

  
  void Print(HDC print_dc, const RECT& print_bounds);

  
  
  void PrintTab();

  void set_use_chrome_network(bool use_chrome_network) {
    use_chrome_network_ = use_chrome_network;
  }

  bool use_chrome_network() const {
    return use_chrome_network_;
  }

#ifdef UNIT_TEST
  void set_proxy_factory(ProxyFactory* factory) {
    proxy_factory_ = factory;
  }
#endif

  void set_handle_top_level_requests(bool handle_top_level_requests) {
    handle_top_level_requests_ = handle_top_level_requests;
  }

  
  void SetUrlFetcher(PluginUrlRequestManager* url_fetcher);

  
  void AttachExternalTab(uint64 external_tab_cookie);
  void BlockExternalTab(uint64 cookie);

  void SetPageFontSize(enum AutomationPageFontSize);

  
  void RemoveBrowsingData(int remove_mask);

  
  void SetZoomLevel(content::PageZoom zoom_level);

  
  
  void OnUnload(bool* should_unload);

 protected:
  
  virtual void LaunchComplete(ChromeFrameAutomationProxy* proxy,
                              AutomationLaunchResult result);
  virtual void AutomationServerDied();

  
  virtual bool OnMessageReceived(TabProxy* tab, const IPC::Message& msg);
  virtual void OnChannelError(TabProxy* tab);

  void CreateExternalTab();
  AutomationLaunchResult CreateExternalTabComplete(HWND chrome_window,
                                                   HWND tab_window,
                                                   int tab_handle,
                                                   int session_id);
  
  
  void InitializeComplete(AutomationLaunchResult result);

  virtual void OnFinalMessage(HWND wnd) {
    Release();
  }

  scoped_refptr<ChromeFrameLaunchParams> launch_params() {
    return chrome_launch_params_;
  }

 private:
  void OnMessageReceivedUIThread(const IPC::Message& msg);
  void OnChannelErrorUIThread();

  HWND chrome_window() const { return chrome_window_; }
  void BeginNavigate();
  void BeginNavigateCompleted(AutomationMsg_NavigationResponseValues result);

  
  void ReportNavigationError(AutomationMsg_NavigationResponseValues error_code,
                             const std::string& url);

  bool ProcessUrlRequestMessage(TabProxy* tab, const IPC::Message& msg,
                                bool ui_thread);

  
  
  virtual void OnResponseStarted(
      int request_id, const char* mime_type, const char* headers, int size,
      base::Time last_modified, const std::string& redirect_url,
      int redirect_status, const net::HostPortPair& socket_address,
      uint64 upload_size);
  virtual void OnReadComplete(int request_id, const std::string& data);
  virtual void OnResponseEnd(int request_id,
                             const net::URLRequestStatus& status);

  bool is_initialized() const {
    return init_state_ == INITIALIZED;
  }

  HWND parent_window_;
  base::PlatformThreadId ui_thread_id_;

  void* automation_server_id_;
  ChromeFrameAutomationProxy* automation_server_;

  HWND chrome_window_;
  scoped_refptr<TabProxy> tab_;
  ChromeFrameDelegate* chrome_frame_delegate_;

  
  
  HWND tab_window_;

  
  std::string automation_server_version_;

  typedef enum InitializationState {
    UNINITIALIZED = 0,
    INITIALIZING,
    INITIALIZED,
    UNINITIALIZING,
  };

  InitializationState init_state_;
  bool use_chrome_network_;
  bool handle_top_level_requests_;
  ProxyFactory* proxy_factory_;
  int tab_handle_;
  
  int session_id_;
  
  uint64 external_tab_cookie_;

  
  
  bool navigate_after_initialization_;

  scoped_refptr<ChromeFrameLaunchParams> chrome_launch_params_;

  
  base::win::ScopedComPtr<IInternetSecurityManager> security_manager_;

  
  
  PluginUrlRequestManager* url_fetcher_;
  PluginUrlRequestManager::ThreadSafeFlags url_fetcher_flags_;

  
  
  
  bool route_all_top_level_navigations_;

  friend class BeginNavigateContext;
  friend class CreateExternalTabContext;
};

#endif  
