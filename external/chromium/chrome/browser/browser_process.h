// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_BROWSER_PROCESS_H_
#define CHROME_BROWSER_BROWSER_PROCESS_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ipc/ipc_message.h"

class AutomationProviderList;

namespace safe_browsing {
class ClientSideDetectionService;
}

class ChromeNetLog;
class DevToolsManager;
class DownloadRequestLimiter;
class DownloadStatusUpdater;
class ExtensionEventRouterForwarder;
class GoogleURLTracker;
class IconManager;
class IntranetRedirectDetector;
class IOThread;
class MetricsService;
class NotificationUIManager;
class PrefService;
class ProfileManager;
class ResourceDispatcherHost;
class SidebarManager;
class TabCloseableStateWatcher;
class ThumbnailGenerator;
class WatchDogThread;

namespace base {
class Thread;
class WaitableEvent;
}

#if defined(OS_CHROMEOS)
namespace chromeos {
class ProxyConfigServiceImpl;
}
#endif  

namespace net {
class URLRequestContextGetter;
}

namespace printing {
class PrintJobManager;
class PrintPreviewTabController;
}

namespace policy {
class BrowserPolicyConnector;
}

namespace ui {
class Clipboard;
}

class BrowserProcess {
 public:
  BrowserProcess();
  virtual ~BrowserProcess();

  
  
  
  
  virtual void EndSession() = 0;

  
  virtual ResourceDispatcherHost* resource_dispatcher_host() = 0;

  virtual MetricsService* metrics_service() = 0;
  virtual ProfileManager* profile_manager() = 0;
  virtual PrefService* local_state() = 0;
  virtual DevToolsManager* devtools_manager() = 0;
  virtual SidebarManager* sidebar_manager() = 0;
  virtual ui::Clipboard* clipboard() = 0;
  virtual net::URLRequestContextGetter* system_request_context() = 0;

#if defined(OS_CHROMEOS)
  
  virtual chromeos::ProxyConfigServiceImpl*
      chromeos_proxy_config_service_impl() = 0;
#endif  

  virtual ExtensionEventRouterForwarder*
      extension_event_router_forwarder() = 0;

  
  virtual NotificationUIManager* notification_ui_manager() = 0;

  
  
  
  
  
  
  
  virtual IOThread* io_thread() = 0;

  
  
  
  
  virtual base::Thread* file_thread() = 0;

  
  
  virtual base::Thread* db_thread() = 0;

  
  virtual base::Thread* cache_thread() = 0;

  
  virtual base::Thread* gpu_thread() = 0;

#if defined(USE_X11)
  
  
  
  
  
  virtual base::Thread* background_x11_thread() = 0;
#endif

  
  virtual WatchDogThread* watchdog_thread() = 0;

  virtual policy::BrowserPolicyConnector* browser_policy_connector() = 0;

  virtual IconManager* icon_manager() = 0;

  virtual ThumbnailGenerator* GetThumbnailGenerator() = 0;

  virtual AutomationProviderList* InitAutomationProviderList() = 0;

  virtual void InitDevToolsHttpProtocolHandler(
      const std::string& ip,
      int port,
      const std::string& frontend_url) = 0;

  virtual void InitDevToolsLegacyProtocolHandler(int port) = 0;

  virtual unsigned int AddRefModule() = 0;
  virtual unsigned int ReleaseModule() = 0;

  virtual bool IsShuttingDown() = 0;

  virtual printing::PrintJobManager* print_job_manager() = 0;
  virtual printing::PrintPreviewTabController*
      print_preview_tab_controller() = 0;

  virtual GoogleURLTracker* google_url_tracker() = 0;
  virtual IntranetRedirectDetector* intranet_redirect_detector() = 0;

  
  virtual const std::string& GetApplicationLocale() = 0;
  virtual void SetApplicationLocale(const std::string& locale) = 0;

  DownloadRequestLimiter* download_request_limiter();
  virtual DownloadStatusUpdater* download_status_updater() = 0;

  
  virtual base::WaitableEvent* shutdown_event() = 0;

  
  std::vector<std::wstring>& user_data_dir_profiles() {
    return user_data_dir_profiles_;
  }

  
  virtual TabCloseableStateWatcher* tab_closeable_state_watcher() = 0;

  
  
  virtual safe_browsing::ClientSideDetectionService*
      safe_browsing_detection_service() = 0;

  
  
  virtual bool plugin_finder_disabled() const = 0;

#if (defined(OS_WIN) || defined(OS_LINUX)) && !defined(OS_CHROMEOS)
  
  
  
  
  
  
  
  virtual void StartAutoupdateTimer() = 0;
#endif

  virtual ChromeNetLog* net_log() = 0;

#if defined(IPC_MESSAGE_LOG_ENABLED)
  
  
  
  virtual void SetIPCLoggingEnabled(bool enable) = 0;
#endif

  const std::string& plugin_data_remover_mime_type() const {
    return plugin_data_remover_mime_type_;
  }

  void set_plugin_data_remover_mime_type(const std::string& mime_type) {
    plugin_data_remover_mime_type_ = mime_type;
  }

 private:
  
  std::vector<std::wstring> user_data_dir_profiles_;

  
  std::string plugin_data_remover_mime_type_;

  DISALLOW_COPY_AND_ASSIGN(BrowserProcess);
};

extern BrowserProcess* g_browser_process;

#endif  
