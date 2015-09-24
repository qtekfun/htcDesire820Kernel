// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_BROWSER_PROCESS_H_
#define CHROME_BROWSER_BROWSER_PROCESS_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/browser_process_platform_part.h"
#include "chrome/browser/ui/host_desktop.h"

class AutomationProviderList;
class BackgroundModeManager;
class BookmarkPromptController;
class ChromeNetLog;
class CRLSetFetcher;
class ComponentUpdateService;
class DownloadRequestLimiter;
class DownloadStatusUpdater;
class GLStringManager;
class GpuModeManager;
class IconManager;
class IntranetRedirectDetector;
class IOThread;
class MediaFileSystemRegistry;
class MetricsService;
class NotificationUIManager;
class PnaclComponentInstaller;
class PrefRegistrySimple;
class PrefService;
class Profile;
class ProfileManager;
class RenderWidgetSnapshotTaker;
class SafeBrowsingService;
class StatusTray;
class StorageMonitor;
class WatchDogThread;
#if defined(ENABLE_WEBRTC)
class WebRtcLogUploader;
#endif

namespace chrome_variations {
class VariationsService;
}

namespace extensions {
class EventRouterForwarder;
}

namespace message_center {
class MessageCenter;
}

namespace net {
class URLRequestContextGetter;
}

namespace policy {
class BrowserPolicyConnector;
class PolicyService;
}

namespace prerender {
class PrerenderTracker;
}

namespace printing {
class BackgroundPrintingManager;
class PrintJobManager;
class PrintPreviewDialogController;
}

namespace safe_browsing {
class ClientSideDetectionService;
}

class BrowserProcess {
 public:
  BrowserProcess();
  virtual ~BrowserProcess();

  
  virtual void ResourceDispatcherHostCreated() = 0;

  
  
  
  
  virtual void EndSession() = 0;

  
  virtual MetricsService* metrics_service() = 0;
  virtual ProfileManager* profile_manager() = 0;
  virtual PrefService* local_state() = 0;
  virtual net::URLRequestContextGetter* system_request_context() = 0;
  virtual chrome_variations::VariationsService* variations_service() = 0;

  virtual BrowserProcessPlatformPart* platform_part() = 0;

  virtual extensions::EventRouterForwarder*
      extension_event_router_forwarder() = 0;

  
  virtual NotificationUIManager* notification_ui_manager() = 0;

  
  virtual message_center::MessageCenter* message_center() = 0;

  
  
  
  
  
  
  
  
  virtual IOThread* io_thread() = 0;

  
  virtual WatchDogThread* watchdog_thread() = 0;

  
  virtual policy::BrowserPolicyConnector* browser_policy_connector() = 0;

  
  
  virtual policy::PolicyService* policy_service() = 0;

  virtual IconManager* icon_manager() = 0;

  virtual GLStringManager* gl_string_manager() = 0;

  virtual GpuModeManager* gpu_mode_manager() = 0;

  virtual RenderWidgetSnapshotTaker* GetRenderWidgetSnapshotTaker() = 0;

  virtual AutomationProviderList* GetAutomationProviderList() = 0;

  virtual void CreateDevToolsHttpProtocolHandler(
      chrome::HostDesktopType host_desktop_type,
      const std::string& ip,
      int port,
      const std::string& frontend_url) = 0;

  virtual unsigned int AddRefModule() = 0;
  virtual unsigned int ReleaseModule() = 0;

  virtual bool IsShuttingDown() = 0;

  virtual printing::PrintJobManager* print_job_manager() = 0;
  virtual printing::PrintPreviewDialogController*
      print_preview_dialog_controller() = 0;
  virtual printing::BackgroundPrintingManager*
      background_printing_manager() = 0;

  virtual IntranetRedirectDetector* intranet_redirect_detector() = 0;

  
  virtual const std::string& GetApplicationLocale() = 0;
  virtual void SetApplicationLocale(const std::string& locale) = 0;

  virtual DownloadStatusUpdater* download_status_updater() = 0;
  virtual DownloadRequestLimiter* download_request_limiter() = 0;

  
  virtual BackgroundModeManager* background_mode_manager() = 0;
  virtual void set_background_mode_manager_for_test(
      scoped_ptr<BackgroundModeManager> manager) = 0;

  
  
  
  virtual StatusTray* status_tray() = 0;

  
  virtual SafeBrowsingService* safe_browsing_service() = 0;

  
  
  virtual safe_browsing::ClientSideDetectionService*
      safe_browsing_detection_service() = 0;

#if (defined(OS_WIN) || defined(OS_LINUX)) && !defined(OS_CHROMEOS)
  
  
  
  
  
  
  
  virtual void StartAutoupdateTimer() = 0;
#endif

  virtual ChromeNetLog* net_log() = 0;

  virtual prerender::PrerenderTracker* prerender_tracker() = 0;

  virtual ComponentUpdateService* component_updater() = 0;

  virtual CRLSetFetcher* crl_set_fetcher() = 0;

  virtual PnaclComponentInstaller* pnacl_component_installer() = 0;

  virtual BookmarkPromptController* bookmark_prompt_controller() = 0;

  virtual MediaFileSystemRegistry* media_file_system_registry() = 0;

  virtual StorageMonitor* storage_monitor() = 0;

  virtual bool created_local_state() const = 0;

#if defined(ENABLE_WEBRTC)
  virtual WebRtcLogUploader* webrtc_log_uploader() = 0;
#endif

 private:
  DISALLOW_COPY_AND_ASSIGN(BrowserProcess);
};

extern BrowserProcess* g_browser_process;

#endif  
