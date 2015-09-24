// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_BROWSER_PROCESS_IMPL_H_
#define CHROME_BROWSER_BROWSER_PROCESS_IMPL_H_

#include <string>

#include "base/basictypes.h"
#include "base/debug/stack_trace.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/threading/non_thread_safe.h"
#include "base/timer/timer.h"
#include "chrome/browser/browser_process.h"

class ChromeNetLog;
class ChromeResourceDispatcherHostDelegate;
class CommandLine;
class RemoteDebuggingServer;
class PrefRegistrySimple;
class PromoResourceService;

#if defined(ENABLE_PLUGIN_INSTALLATION)
class PluginsResourceService;
#endif

namespace base {
class SequencedTaskRunner;
}

namespace extensions {
class ExtensionsBrowserClient;
}

namespace policy {
class BrowserPolicyConnector;
class PolicyService;
};

class BrowserProcessImpl : public BrowserProcess,
                           public base::NonThreadSafe {
 public:
  
  BrowserProcessImpl(base::SequencedTaskRunner* local_state_task_runner,
                     const CommandLine& command_line);
  virtual ~BrowserProcessImpl();

  
  void PreCreateThreads();

  
  
  
  void PreMainMessageLoopRun();

  
  
  
  
  void StartTearDown();
  void PostDestroyThreads();

  
  virtual void ResourceDispatcherHostCreated() OVERRIDE;
  virtual void EndSession() OVERRIDE;
  virtual MetricsService* metrics_service() OVERRIDE;
  virtual IOThread* io_thread() OVERRIDE;
  virtual WatchDogThread* watchdog_thread() OVERRIDE;
  virtual ProfileManager* profile_manager() OVERRIDE;
  virtual PrefService* local_state() OVERRIDE;
  virtual net::URLRequestContextGetter* system_request_context() OVERRIDE;
  virtual chrome_variations::VariationsService* variations_service() OVERRIDE;
  virtual BrowserProcessPlatformPart* platform_part() OVERRIDE;
  virtual extensions::EventRouterForwarder*
        extension_event_router_forwarder() OVERRIDE;
  virtual NotificationUIManager* notification_ui_manager() OVERRIDE;
  virtual message_center::MessageCenter* message_center() OVERRIDE;
  virtual policy::BrowserPolicyConnector* browser_policy_connector() OVERRIDE;
  virtual policy::PolicyService* policy_service() OVERRIDE;
  virtual IconManager* icon_manager() OVERRIDE;
  virtual GLStringManager* gl_string_manager() OVERRIDE;
  virtual GpuModeManager* gpu_mode_manager() OVERRIDE;
  virtual RenderWidgetSnapshotTaker* GetRenderWidgetSnapshotTaker() OVERRIDE;
  virtual AutomationProviderList* GetAutomationProviderList() OVERRIDE;
  virtual void CreateDevToolsHttpProtocolHandler(
      chrome::HostDesktopType host_desktop_type,
      const std::string& ip,
      int port,
      const std::string& frontend_url) OVERRIDE;
  virtual unsigned int AddRefModule() OVERRIDE;
  virtual unsigned int ReleaseModule() OVERRIDE;
  virtual bool IsShuttingDown() OVERRIDE;
  virtual printing::PrintJobManager* print_job_manager() OVERRIDE;
  virtual printing::PrintPreviewDialogController*
      print_preview_dialog_controller() OVERRIDE;
  virtual printing::BackgroundPrintingManager*
      background_printing_manager() OVERRIDE;
  virtual IntranetRedirectDetector* intranet_redirect_detector() OVERRIDE;
  virtual const std::string& GetApplicationLocale() OVERRIDE;
  virtual void SetApplicationLocale(const std::string& locale) OVERRIDE;
  virtual DownloadStatusUpdater* download_status_updater() OVERRIDE;
  virtual DownloadRequestLimiter* download_request_limiter() OVERRIDE;
  virtual BackgroundModeManager* background_mode_manager() OVERRIDE;
  virtual void set_background_mode_manager_for_test(
      scoped_ptr<BackgroundModeManager> manager) OVERRIDE;
  virtual StatusTray* status_tray() OVERRIDE;
  virtual SafeBrowsingService* safe_browsing_service() OVERRIDE;
  virtual safe_browsing::ClientSideDetectionService*
      safe_browsing_detection_service() OVERRIDE;

#if (defined(OS_WIN) || defined(OS_LINUX)) && !defined(OS_CHROMEOS)
  virtual void StartAutoupdateTimer() OVERRIDE;
#endif

  virtual ChromeNetLog* net_log() OVERRIDE;
  virtual prerender::PrerenderTracker* prerender_tracker() OVERRIDE;
  virtual ComponentUpdateService* component_updater() OVERRIDE;
  virtual CRLSetFetcher* crl_set_fetcher() OVERRIDE;
  virtual PnaclComponentInstaller* pnacl_component_installer() OVERRIDE;
  virtual BookmarkPromptController* bookmark_prompt_controller() OVERRIDE;
  virtual StorageMonitor* storage_monitor() OVERRIDE;
  void set_storage_monitor_for_test(scoped_ptr<StorageMonitor> monitor);
  virtual MediaFileSystemRegistry* media_file_system_registry() OVERRIDE;
  virtual bool created_local_state() const OVERRIDE;
#if defined(ENABLE_WEBRTC)
  virtual WebRtcLogUploader* webrtc_log_uploader() OVERRIDE;
#endif

  static void RegisterPrefs(PrefRegistrySimple* registry);

 private:
  void CreateMetricsService();
  void CreateWatchdogThread();
  void CreateProfileManager();
  void CreateLocalState();
  void CreateViewedPageTracker();
  void CreateIconManager();
  void CreateIntranetRedirectDetector();
  void CreateNotificationUIManager();
  void CreateStatusTrayManager();
  void CreatePrintPreviewDialogController();
  void CreateBackgroundPrintingManager();
  void CreateSafeBrowsingService();
  void CreateSafeBrowsingDetectionService();
  void CreateStatusTray();
  void CreateBackgroundModeManager();

  void ApplyAllowCrossOriginAuthPromptPolicy();
  void ApplyDefaultBrowserPolicy();
  void ApplyMetricsReportingPolicy();

  bool created_metrics_service_;
  scoped_ptr<MetricsService> metrics_service_;

  scoped_ptr<IOThread> io_thread_;

  bool created_watchdog_thread_;
  scoped_ptr<WatchDogThread> watchdog_thread_;

  bool created_browser_policy_connector_;
#if defined(ENABLE_CONFIGURATION_POLICY)
  
  scoped_ptr<policy::BrowserPolicyConnector> browser_policy_connector_;
#endif

  
  
  
  scoped_ptr<policy::PolicyService> policy_service_;

  bool created_profile_manager_;
  scoped_ptr<ProfileManager> profile_manager_;

  bool created_local_state_;
  scoped_ptr<PrefService> local_state_;

  bool created_icon_manager_;
  scoped_ptr<IconManager> icon_manager_;

  scoped_ptr<GLStringManager> gl_string_manager_;

  scoped_ptr<GpuModeManager> gpu_mode_manager_;

  scoped_ptr<extensions::ExtensionsBrowserClient> extensions_browser_client_;
  scoped_refptr<extensions::EventRouterForwarder>
      extension_event_router_forwarder_;

#if !defined(OS_ANDROID)
  scoped_ptr<RemoteDebuggingServer> remote_debugging_server_;

  
  scoped_ptr<BookmarkPromptController> bookmark_prompt_controller_;
#endif

#if !defined(OS_ANDROID) && !defined(OS_IOS)
  scoped_ptr<StorageMonitor> storage_monitor_;

  scoped_ptr<MediaFileSystemRegistry> media_file_system_registry_;
#endif

  scoped_refptr<printing::PrintPreviewDialogController>
      print_preview_dialog_controller_;

  scoped_ptr<printing::BackgroundPrintingManager> background_printing_manager_;

  scoped_ptr<chrome_variations::VariationsService> variations_service_;

  
  bool created_notification_ui_manager_;
  scoped_ptr<NotificationUIManager> notification_ui_manager_;

#if defined(ENABLE_AUTOMATION)
  scoped_ptr<AutomationProviderList> automation_provider_list_;
#endif

  scoped_ptr<IntranetRedirectDetector> intranet_redirect_detector_;

  scoped_ptr<StatusTray> status_tray_;

  scoped_ptr<BackgroundModeManager> background_mode_manager_;

  bool created_safe_browsing_service_;
  scoped_refptr<SafeBrowsingService> safe_browsing_service_;

  unsigned int module_ref_count_;
  bool did_start_;

  
  scoped_ptr<printing::PrintJobManager> print_job_manager_;

  std::string locale_;

  bool checked_for_new_frames_;
  bool using_new_frames_;

  
  
  scoped_ptr<RenderWidgetSnapshotTaker> render_widget_snapshot_taker_;

  
  
  
  scoped_ptr<DownloadStatusUpdater> download_status_updater_;

  scoped_refptr<DownloadRequestLimiter> download_request_limiter_;

  
  const scoped_refptr<base::SequencedTaskRunner> local_state_task_runner_;

  
  
  PrefChangeRegistrar pref_change_registrar_;

  
  scoped_ptr<ChromeNetLog> net_log_;

  
  
  scoped_ptr<prerender::PrerenderTracker> prerender_tracker_;

  scoped_ptr<ChromeResourceDispatcherHostDelegate>
      resource_dispatcher_host_delegate_;

  scoped_refptr<PromoResourceService> promo_resource_service_;

#if (defined(OS_WIN) || defined(OS_LINUX)) && !defined(OS_CHROMEOS)
  base::RepeatingTimer<BrowserProcessImpl> autoupdate_timer_;

  
  
  void OnAutoupdateTimer();
  bool CanAutorestartForUpdate() const;
  void RestartBackgroundInstance();
#endif  

  
  
  
  scoped_ptr<ComponentUpdateService> component_updater_;
  scoped_refptr<CRLSetFetcher> crl_set_fetcher_;
  scoped_ptr<PnaclComponentInstaller> pnacl_component_installer_;

#if defined(ENABLE_PLUGIN_INSTALLATION)
  scoped_refptr<PluginsResourceService> plugins_resource_service_;
#endif

  scoped_ptr<BrowserProcessPlatformPart> platform_part_;

  
  
  base::debug::StackTrace release_last_reference_callstack_;

#if defined(ENABLE_WEBRTC)
  
  scoped_ptr<WebRtcLogUploader> webrtc_log_uploader_;
#endif

  DISALLOW_COPY_AND_ASSIGN(BrowserProcessImpl);
};

#endif  
