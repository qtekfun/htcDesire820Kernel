// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_METRICS_METRICS_SERVICE_H_
#define CHROME_BROWSER_METRICS_METRICS_SERVICE_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/common/metrics_helpers.h"
#include "chrome/common/net/url_fetcher.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

#if defined(OS_CHROMEOS)
#include "chrome/browser/chromeos/external_metrics.h"
#endif

class BookmarkModel;
class BookmarkNode;
class DictionaryValue;
class ListValue;
class HistogramSynchronizer;
class MetricsLogBase;
class PrefService;
class TemplateURLModel;

namespace webkit {
namespace npapi {
struct WebPluginInfo;
}
}

struct _xmlNode;
typedef struct _xmlNode xmlNode;
typedef xmlNode* xmlNodePtr;


class MetricsService : public NotificationObserver,
                       public URLFetcher::Delegate,
                       public MetricsServiceBase {
 public:
  
  
  enum LogRecallStatus {
    RECALL_SUCCESS,         
    LIST_EMPTY,             
    LIST_SIZE_MISSING,      
    LIST_SIZE_TOO_SMALL,    
    LIST_SIZE_CORRUPTION,   
    LOG_STRING_CORRUPTION,  
    CHECKSUM_CORRUPTION,    
    CHECKSUM_STRING_CORRUPTION,  
                                 
    DECODE_FAIL,            
    END_RECALL_STATUS       
  };

  
  
  enum LogStoreStatus {
    STORE_SUCCESS,    
    ENCODE_FAIL,      
    COMPRESS_FAIL,    
    END_STORE_STATUS  
  };

  MetricsService();
  virtual ~MetricsService();

  
  
  
  
  void Start();
  void StartRecordingOnly();
  void Stop();

  
  
  static void RegisterPrefs(PrefService* local_state);

  
  
  
  static void SetUpNotifications(NotificationRegistrar* registrar,
                                 NotificationObserver* observer);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  void RecordStartOfSessionEnd();

  
  
  void RecordCompletedSessionEnd();

  
  
  void RecordBreakpadRegistration(bool success);

  
  
  void RecordBreakpadHasDebugger(bool has_debugger);

  
  
  void StoreUnsentLogs();

#if defined(OS_CHROMEOS)
  
  
  void StartExternalMetrics();

  
  void LogChromeOSCrash(const std::string &crash_type);
#endif

  bool recording_active() const;
  bool reporting_active() const;

 private:
  
  
  enum State {
    INITIALIZED,            
    INIT_TASK_SCHEDULED,    
    INIT_TASK_DONE,         
    INITIAL_LOG_READY,      
    SEND_OLD_INITIAL_LOGS,  
    SENDING_OLD_LOGS,       
    SENDING_CURRENT_LOGS,   
  };

  class InitTask;
  class InitTaskComplete;

  
  void OnInitTaskComplete(
      const std::string& hardware_class,
      const std::vector<webkit::npapi::WebPluginInfo>& plugins);

  
  
  
  
  
  
  static void DiscardOldStabilityStats(PrefService* local_state);

  
  
  
  void SetRecording(bool enabled);

  
  void SetReporting(bool enabled);

  
  
  
  
  void HandleIdleSinceLastTransmission(bool in_idle);

  
  void InitializeMetricsState();

  
  static std::string GenerateClientID();

  
  
  void ScheduleNextStateSave();

  
  
  
  void SaveLocalState();

  
  
  void StartRecording();

  
  
  
  void StopRecording(MetricsLogBase** log);

  
  
  void PushPendingLogsToUnsentLists();

  
  
  void PushPendingLogTextToUnsentOngoingLogs();

  
  void StartLogTransmissionTimer();

  
  void LogTransmissionTimerDone();

  
  
  
  void OnMemoryDetailCollectionDone();
  void OnHistogramSynchronizationDone();

  
  
  
  void MakePendingLog();

  
  
  bool ServerPermitsTransmission() const;

  
  bool unsent_logs() const {
    return !unsent_initial_logs_.empty() || !unsent_ongoing_logs_.empty();
  }
  
  void PrepareInitialLog();
  
  void RecallUnsentLogs();
  // Decode and verify written pref log data.
  static MetricsService::LogRecallStatus RecallUnsentLogsHelper(
      const ListValue& list,
      std::vector<std::string>* local_list);
  
  static void StoreUnsentLogsHelper(const std::vector<std::string>& local_list,
                                    const size_t kMaxLocalListSize,
                                    ListValue* list);
  
  
  void PreparePendingLogText();

  
  
  
  void PrepareFetchWithPendingLog();

  
  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  
  
  void HandleBadResponseCode();

  
  void LogWindowChange(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void IncrementPrefValue(const char* path);

  
  
  void IncrementLongPrefsValue(const char* path);

  
  void LogRendererCrash();

  
  void LogExtensionRendererCrash();

  
  void LogRendererHang();

  
  void LogCleanShutdown();

  
  
  
  void LogBookmarks(const BookmarkNode* node,
                    const char* num_bookmarks_key,
                    const char* num_folders_key);

  
  void LogBookmarks(BookmarkModel* model);

  
  
  
  void LogChildProcessChange(NotificationType type,
                             const NotificationSource& source,
                             const NotificationDetails& details);

  
  
  void LogKeywords(const TemplateURLModel* url_model);

  
  
  void RecordPluginChanges(PrefService* pref);

  
  
  void RecordCurrentState(PrefService* pref);

  
  void LogLoadStarted();

  
  void LogLoadComplete(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  bool CanLogNotification(NotificationType type,
                          const NotificationSource& source,
                          const NotificationDetails& details);

  
  void RecordBooleanPrefValue(const char* path, bool value);

  NotificationRegistrar registrar_;

  
  
  
  bool recording_active_;
  bool reporting_active_;

  
  
  
  
  bool server_permits_upload_;

  
  
  State state_;

  
  
  
  
  std::string hardware_class_;

  
  std::vector<webkit::npapi::WebPluginInfo> plugins_;

  
  scoped_ptr<URLFetcher> current_fetch_;

  
  std::wstring server_url_;

  
  std::string client_id_;

  
  
  bool idle_since_last_transmission_;

  
  int session_id_;

  
  
  
  
  
  
  
  std::vector<std::string> unsent_initial_logs_;
  std::vector<std::string> unsent_ongoing_logs_;

  
  
  
  typedef std::map<uintptr_t, int> WindowMap;
  WindowMap window_map_;
  int next_window_id_;

  
  
  struct ChildProcessStats;
  std::map<std::wstring, ChildProcessStats> child_process_stats_buffer_;

  ScopedRunnableMethodFactory<MetricsService> log_sender_factory_;
  ScopedRunnableMethodFactory<MetricsService> state_saver_factory_;

  
  
  scoped_ptr<DictionaryValue> profile_dictionary_;

  
  
  
  
  base::TimeDelta interlog_duration_;

  
  bool timer_pending_;

#if defined(OS_CHROMEOS)
  
  scoped_refptr<chromeos::ExternalMetrics> external_metrics_;
#endif

  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, EmptyLogList);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, SingleElementLogList);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, OverLimitLogList);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, SmallRecoveredListSize);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, RemoveSizeFromLogList);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, CorruptSizeOfLogList);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, CorruptChecksumOfLogList);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, ClientIdGeneratesAllZeroes);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, ClientIdGeneratesCorrectly);
  FRIEND_TEST_ALL_PREFIXES(MetricsServiceTest, ClientIdCorrectlyFormatted);

  DISALLOW_COPY_AND_ASSIGN(MetricsService);
};

#endif  
