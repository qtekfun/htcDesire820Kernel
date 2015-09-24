// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_METRICS_METRICS_SERVICE_H_
#define CHROME_BROWSER_METRICS_METRICS_SERVICE_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/metrics/field_trial.h"
#include "base/process/kill.h"
#include "base/time/time.h"
#include "chrome/browser/metrics/metrics_log.h"
#include "chrome/browser/metrics/tracking_synchronizer_observer.h"
#include "chrome/common/metrics/metrics_service_base.h"
#include "chrome/installer/util/google_update_settings.h"
#include "content/public/browser/browser_child_process_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/user_metrics.h"
#include "net/url_request/url_fetcher_delegate.h"

#if defined(OS_CHROMEOS)
#include "chrome/browser/chromeos/external_metrics.h"
#endif

class MetricsReportingScheduler;
class PrefService;
class PrefRegistrySimple;
class Profile;
class TemplateURLService;

namespace base {
class DictionaryValue;
class MessageLoopProxy;
}

namespace chrome_variations {
struct ActiveGroupId;
}

namespace content {
class RenderProcessHost;
class WebContents;
struct WebPluginInfo;
}

namespace extensions {
class ExtensionDownloader;
class ManifestFetchData;
}

namespace net {
class URLFetcher;
}

namespace prerender {
bool IsOmniboxEnabled(Profile* profile);
}

namespace tracked_objects {
struct ProcessDataSnapshot;
}

struct SyntheticTrialGroup {
 public:
  ~SyntheticTrialGroup();

  chrome_variations::ActiveGroupId id;
  base::TimeTicks start_time;

 private:
  friend class MetricsService;
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, RegisterSyntheticTrial);

  
  
  
  SyntheticTrialGroup(uint32 trial, uint32 group, base::TimeTicks start);

};

class MetricsService
    : public chrome_browser_metrics::TrackingSynchronizerObserver,
      public content::BrowserChildProcessObserver,
      public content::NotificationObserver,
      public net::URLFetcherDelegate,
      public MetricsServiceBase {
 public:
  
  enum ExecutionPhase {
    UNINITIALIZED_PHASE = 0,
    START_METRICS_RECORDING = 100,
    CREATE_PROFILE = 200,
    STARTUP_TIMEBOMB_ARM = 300,
    THREAD_WATCHER_START = 400,
    MAIN_MESSAGE_LOOP_RUN = 500,
    SHUTDOWN_TIMEBOMB_ARM = 600,
    SHUTDOWN_COMPLETE = 700,
  };

  enum ReportingState {
    REPORTING_ENABLED,
    REPORTING_DISABLED,
  };

  MetricsService();
  virtual ~MetricsService();

  
  
  
  
  
  void InitializeMetricsRecordingState(ReportingState reporting_state);

  
  
  
  void Start();

  
  
  
  void StartRecordingForTests();

  
  
  void Stop();

  
  
  
  
  void EnableReporting();
  void DisableReporting();

  
  
  std::string GetClientId();

  
  
  
  
  
  
  
  
  
  
  
  
  
  scoped_ptr<const base::FieldTrial::EntropyProvider> CreateEntropyProvider(
      ReportingState reporting_state);

  
  
  void ForceClientIdCreation();

  
  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  
  
  static void SetUpNotifications(content::NotificationRegistrar* registrar,
                                 content::NotificationObserver* observer);

  
  virtual void BrowserChildProcessHostConnected(
      const content::ChildProcessData& data) OVERRIDE;
  virtual void BrowserChildProcessCrashed(
      const content::ChildProcessData& data) OVERRIDE;
  virtual void BrowserChildProcessInstanceCreated(
      const content::ChildProcessData& data) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void RecordStartOfSessionEnd();

  
  
  void RecordCompletedSessionEnd();

#if defined(OS_ANDROID) || defined(OS_IOS)
  
  void OnAppEnterBackground();

  
  void OnAppEnterForeground();
#else
  
  static void LogNeedForCleanShutdown();
#endif  

  static void SetExecutionPhase(ExecutionPhase execution_phase);

  
  
  void RecordBreakpadRegistration(bool success);

  
  
  void RecordBreakpadHasDebugger(bool has_debugger);

#if defined(OS_WIN)
  
  
  void CountBrowserCrashDumpAttempts();
#endif  

#if defined(OS_CHROMEOS)
  
  
  void StartExternalMetrics();

  
  void LogChromeOSCrash(const std::string &crash_type);
#endif

  bool recording_active() const;
  bool reporting_active() const;

  void LogPluginLoadingError(const base::FilePath& plugin_path);

  
  
  static bool UmaMetricsProperlyShutdown();

  
  
  
  
  
  
  
  
  void RegisterSyntheticFieldTrial(const SyntheticTrialGroup& trial_group);

 private:
  
  
  enum State {
    INITIALIZED,                    
    INIT_TASK_SCHEDULED,            
                                    
    INIT_TASK_DONE,                 
    SENDING_INITIAL_STABILITY_LOG,  
    SENDING_INITIAL_METRICS_LOG,    
    SENDING_OLD_LOGS,               
    SENDING_CURRENT_LOGS,           
  };

  enum ShutdownCleanliness {
    CLEANLY_SHUTDOWN = 0xdeadbeef,
    NEED_TO_SHUTDOWN = ~CLEANLY_SHUTDOWN
  };

  
  
  
  enum EntropySourceReturned {
    LAST_ENTROPY_NONE,
    LAST_ENTROPY_LOW,
    LAST_ENTROPY_HIGH,
  };

  struct ChildProcessStats;

  typedef std::vector<SyntheticTrialGroup> SyntheticTrialGroups;

  
  
  static void InitTaskGetHardwareClass(base::WeakPtr<MetricsService> self,
                                       base::MessageLoopProxy* target_loop);

  
  
  void OnInitTaskGotHardwareClass(const std::string& hardware_class);

  
  
  void OnInitTaskGotPluginInfo(
      const std::vector<content::WebPluginInfo>& plugins);

  
  
  static void InitTaskGetGoogleUpdateData(base::WeakPtr<MetricsService> self,
                                          base::MessageLoopProxy* target_loop);

  
  
  void OnInitTaskGotGoogleUpdateData(
      const GoogleUpdateMetrics& google_update_metrics);

  void OnUserAction(const std::string& action);

  
  virtual void ReceivedProfilerData(
      const tracked_objects::ProcessDataSnapshot& process_data,
      int process_type) OVERRIDE;
  
  virtual void FinishedReceivingProfilerData() OVERRIDE;

  
  
  base::TimeDelta GetIncrementalUptime(PrefService* pref);

  
  
  
  int GetLowEntropySource();

  
  
  
  EntropySourceReturned entropy_source_returned() const {
    return entropy_source_returned_;
  }

  
  
  
  
  
  
  static void DiscardOldStabilityStats(PrefService* local_state);

  
  
  
  void EnableRecording();
  void DisableRecording();

  
  
  
  
  void HandleIdleSinceLastTransmission(bool in_idle);

  
  void InitializeMetricsState(ReportingState reporting_state);

  
  static std::string GenerateClientID();

  
  
  void ScheduleNextStateSave();

  
  
  
  void SaveLocalState();

  
  void OpenNewLog();

  
  void CloseCurrentLog();

  
  
  void PushPendingLogsToPersistentStorage();

  
  
  void StartSchedulerIfNecessary();

  
  void StartScheduledUpload();

  
  
  void StartFinalLogInfoCollection();
  
  
  void OnMemoryDetailCollectionDone();
  void OnHistogramSynchronizationDone();
  void OnFinalLogInfoCollectionDone();

  
  
  void StageNewLog();

  
  
  
  
  void PrepareInitialStabilityLog();

  
  
  void PrepareInitialMetricsLog(MetricsLog::LogType log_type);

  
  void SendStagedLog();

  
  
  
  void PrepareFetchWithStagedLog();

  
  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  void IncrementPrefValue(const char* path);

  
  
  void IncrementLongPrefsValue(const char* path);

  
  void LogRendererCrash(content::RenderProcessHost* host,
                        base::TerminationStatus status,
                        int exit_code);

  
  void LogRendererHang();

  
  void LogCleanShutdown();

  
  ChildProcessStats& GetChildProcessStats(
      const content::ChildProcessData& data);

  
  
  void RecordPluginChanges(PrefService* pref);

  
  
  void RecordCurrentState(PrefService* pref);

  
  
  void LogLoadStarted(content::WebContents* web_contents);

  
  bool CanLogNotification();

  
  void RecordBooleanPrefValue(const char* path, bool value);

  
  
  static bool IsPluginProcess(int process_type);

  
  
  void GetCurrentSyntheticFieldTrials(
      std::vector<chrome_variations::ActiveGroupId>* synthetic_trials);

  content::ActionCallback action_callback_;

  content::NotificationRegistrar registrar_;

  
  
  
  bool recording_active_;
  bool reporting_active_;

  
  
  bool test_mode_active_;

  
  
  State state_;

  
  bool has_initial_stability_log_;

  
  
  
  
  std::string hardware_class_;

  
  std::vector<content::WebPluginInfo> plugins_;

  
  GoogleUpdateMetrics google_update_metrics_;

  
  
  
  scoped_ptr<MetricsLog> initial_metrics_log_;

  
  scoped_ptr<net::URLFetcher> current_fetch_;

  
  std::string network_stats_server_;

  
  std::string http_pipelining_test_server_;

  
  std::string client_id_;

  
  int low_entropy_source_;

  
  
  bool idle_since_last_transmission_;

  
  int session_id_;

  
  
  
  typedef std::map<uintptr_t, int> WindowMap;
  WindowMap window_map_;
  int next_window_id_;

  
  std::map<base::string16, ChildProcessStats> child_process_stats_buffer_;

  
  
  base::WeakPtrFactory<MetricsService> self_ptr_factory_;

  
  
  base::WeakPtrFactory<MetricsService> state_saver_factory_;

  
  scoped_ptr<MetricsReportingScheduler> scheduler_;

  
  
  bool waiting_for_asynchronous_reporting_step_;

  
  int num_async_histogram_fetches_in_progress_;

#if defined(OS_CHROMEOS)
  
  scoped_refptr<chromeos::ExternalMetrics> external_metrics_;
#endif

  
  EntropySourceReturned entropy_source_returned_;

  
  base::TimeTicks last_updated_time_;

  
  static ExecutionPhase execution_phase_;

  
  
  static ShutdownCleanliness clean_shutdown_status_;

  
  SyntheticTrialGroups synthetic_trial_groups_;

  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, ClientIdCorrectlyFormatted);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, IsPluginProcess);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, LowEntropySource0NotReset);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest,
                           PermutedEntropyCacheClearedWhenLowEntropyReset);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, RegisterSyntheticTrial);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceBrowserTest,
                           CheckLowEntropySourceUsed);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceReportingTest,
                           CheckHighEntropySourceUsed);

  DISALLOW_COPY_AND_ASSIGN(MetricsService);
};

class MetricsServiceHelper {
 private:
  friend bool prerender::IsOmniboxEnabled(Profile* profile);
  friend class extensions::ExtensionDownloader;
  friend class extensions::ManifestFetchData;

  
  static bool IsMetricsReportingEnabled();

  DISALLOW_IMPLICIT_CONSTRUCTORS(MetricsServiceHelper);
};

#endif  
