// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PERFORMANCE_MONITOR_PERFORMANCE_MONITOR_H_
#define CHROME_BROWSER_PERFORMANCE_MONITOR_PERFORMANCE_MONITOR_H_

#include <map>
#include <set>
#include <string>

#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/process_handle.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/performance_monitor/event_type.h"
#include "chrome/browser/performance_monitor/process_metrics_history.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/render_process_host.h"

template <typename Type>
struct DefaultSingletonTraits;

namespace extensions {
class Extension;
}

namespace net {
class URLRequest;
}

namespace performance_monitor {
class Database;
class Event;
struct Metric;

class PerformanceMonitor : public content::NotificationObserver {
 public:
  struct PerformanceDataForIOThread {
    PerformanceDataForIOThread();

    uint64 network_bytes_read;
  };

  typedef std::map<base::ProcessHandle, ProcessMetricsHistory> MetricsMap;

  
  
  
  
  
  bool SetDatabasePath(const base::FilePath& path);

  bool database_logging_enabled() const { return database_logging_enabled_; }

  
  
  static PerformanceMonitor* GetInstance();

  
  
  void Initialize();

  
  void StartGatherCycle();

  
  
  
  void BytesReadOnIOThread(const net::URLRequest& request, const int bytes);

  
  
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  Database* database() { return database_.get(); }
  base::FilePath database_path() { return database_path_; }
  static bool initialized() { return initialized_; }

 private:
  friend struct DefaultSingletonTraits<PerformanceMonitor>;
  friend class PerformanceMonitorBrowserTest;
  FRIEND_TEST_ALL_PREFIXES(PerformanceMonitorUncleanExitBrowserTest,
                           OneProfileUncleanExit);
  FRIEND_TEST_ALL_PREFIXES(PerformanceMonitorUncleanExitBrowserTest,
                           TwoProfileUncleanExit);
  FRIEND_TEST_ALL_PREFIXES(PerformanceMonitorBrowserTest, NetworkBytesRead);

  PerformanceMonitor();
  virtual ~PerformanceMonitor();

  
  
  void InitOnBackgroundThread();

  void FinishInit();

  
  void InitializeDatabaseIfNeeded();

  
  void RegisterForNotifications();

  
  
  
  void CheckForUncleanExits();

  
  
  void AddUncleanExitEventOnBackgroundThread(const std::string& profile_name);

  
  
  void CheckForVersionUpdateOnBackgroundThread();

  
  void AddEvent(scoped_ptr<Event> event);

  void AddEventOnBackgroundThread(scoped_ptr<Event> event);

  
  
  void AddMetricOnBackgroundThread(const Metric& metric);

  
  void NotifyInitialized();

  
  void DoTimedCollections();

  
  
#if !defined(OS_ANDROID)
  void UpdateLiveProfiles();
  void UpdateLiveProfilesHelper(
      scoped_ptr<std::set<std::string> > active_profiles, std::string time);
#endif

  
  void StoreMetricsOnBackgroundThread(
      int current_update_sequence,
      const PerformanceDataForIOThread& performance_data_for_io_thread);

  
  
  
  void MarkProcessAsAlive(const base::ProcessHandle& handle,
                          int process_type,
                          int current_update_sequence);

  
  
  void GatherMetricsMapOnUIThread();
  void GatherMetricsMapOnIOThread(int current_update_sequence);

  
  
  void AddExtensionEvent(EventType type,
                         const extensions::Extension* extension);

  
  
  void AddRendererClosedEvent(
      content::RenderProcessHost* host,
      const content::RenderProcessHost::RendererClosedDetails& details);

  
  
  PerformanceDataForIOThread performance_data_for_io_thread_;

  
  
  base::FilePath database_path_;

  scoped_ptr<Database> database_;

  
  MetricsMap metrics_map_;

  
  
  base::Time next_collection_time_;

  
  int gather_interval_in_seconds_;

  
  bool database_logging_enabled_;

  
  base::DelayTimer<PerformanceMonitor> timer_;

  content::NotificationRegistrar registrar_;

  
  
  
  
  static bool initialized_;

  
  bool disable_timer_autostart_for_testing_;

  DISALLOW_COPY_AND_ASSIGN(PerformanceMonitor);
};

}  

#endif  
