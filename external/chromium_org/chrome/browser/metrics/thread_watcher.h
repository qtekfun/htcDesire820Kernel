// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_METRICS_THREAD_WATCHER_H_
#define CHROME_BROWSER_METRICS_THREAD_WATCHER_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/command_line.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/metrics/histogram.h"
#include "base/synchronization/lock.h"
#include "base/threading/platform_thread.h"
#include "base/threading/thread.h"
#include "base/threading/watchdog.h"
#include "base/time/time.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class CustomThreadWatcher;
class StartupTimeBomb;
class ThreadWatcherList;
class ThreadWatcherObserver;

class ThreadWatcher {
 public:
  
  
  struct WatchingParams {
    const content::BrowserThread::ID& thread_id;
    const std::string& thread_name;
    const base::TimeDelta& sleep_time;
    const base::TimeDelta& unresponsive_time;
    uint32 unresponsive_threshold;
    bool crash_on_hang;
    uint32 live_threads_threshold;

    WatchingParams(const content::BrowserThread::ID& thread_id_in,
                   const std::string& thread_name_in,
                   const base::TimeDelta& sleep_time_in,
                   const base::TimeDelta& unresponsive_time_in,
                   uint32 unresponsive_threshold_in,
                   bool crash_on_hang_in,
                   uint32 live_threads_threshold_in)
        : thread_id(thread_id_in),
          thread_name(thread_name_in),
          sleep_time(sleep_time_in),
          unresponsive_time(unresponsive_time_in),
          unresponsive_threshold(unresponsive_threshold_in),
          crash_on_hang(crash_on_hang_in),
          live_threads_threshold(live_threads_threshold_in) {
    }
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  static void StartWatching(const WatchingParams& params);

  
  content::BrowserThread::ID thread_id() const { return thread_id_; }

  
  std::string thread_name() const { return thread_name_; }

  
  base::TimeDelta sleep_time() const { return sleep_time_; }

  
  base::TimeDelta unresponsive_time() const { return unresponsive_time_; }

  
  bool active() const { return active_; }

  
  base::TimeTicks ping_time() const { return ping_time_; }

  
  uint64 ping_sequence_number() const { return ping_sequence_number_; }

 protected:
  
  
  
  explicit ThreadWatcher(const WatchingParams& params);

  virtual ~ThreadWatcher();

  
  virtual void ActivateThreadWatching();

  
  virtual void DeActivateThreadWatching();

  
  
  
  
  virtual void WakeUp();

  
  
  
  
  
  
  virtual void PostPingMessage();

  
  
  
  
  
  virtual void OnPongMessage(uint64 ping_sequence_number);

  
  
  
  
  
  virtual void OnCheckResponsiveness(uint64 ping_sequence_number);

  
  
  bool responsive_;

 private:
  friend class ThreadWatcherList;
  friend class CustomThreadWatcher;

  
  FRIEND_TEST_ALL_PREFIXES(ThreadWatcherTest, Registration);
  FRIEND_TEST_ALL_PREFIXES(ThreadWatcherTest, ThreadResponding);
  FRIEND_TEST_ALL_PREFIXES(ThreadWatcherTest, ThreadNotResponding);
  FRIEND_TEST_ALL_PREFIXES(ThreadWatcherTest, MultipleThreadsResponding);
  FRIEND_TEST_ALL_PREFIXES(ThreadWatcherTest, MultipleThreadsNotResponding);

  
  void Initialize();

  
  
  static void OnPingMessage(const content::BrowserThread::ID& thread_id,
                            const base::Closure& callback_task);

  
  
  void ResetHangCounters();

  
  
  void GotNoResponse();

  
  
  bool IsVeryUnresponsive();

  
  
  const content::BrowserThread::ID thread_id_;

  
  const std::string thread_name_;

  
  scoped_refptr<base::MessageLoopProxy> watched_loop_;

  
  
  const base::TimeDelta sleep_time_;

  
  
  const base::TimeDelta unresponsive_time_;

  
  base::TimeTicks ping_time_;

  
  base::TimeTicks pong_time_;

  
  
  
  uint64 ping_sequence_number_;

  
  bool active_;

  
  
  
  
  int ping_count_;

  
  base::HistogramBase* response_time_histogram_;

  
  
  base::HistogramBase* unresponsive_time_histogram_;

  
  
  base::HistogramBase* responsive_count_histogram_;

  
  
  
  base::HistogramBase* unresponsive_count_histogram_;

  
  
  
  
  uint32 unresponsive_count_;

  
  
  
  bool hung_processing_complete_;

  
  
  
  uint32 unresponsive_threshold_;

  
  
  
  bool crash_on_hang_;

  
  
  
  uint32 live_threads_threshold_;

  
  
  
  base::WeakPtrFactory<ThreadWatcher> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ThreadWatcher);
};

class ThreadWatcherList {
 public:
  
  typedef std::map<content::BrowserThread::ID, ThreadWatcher*> RegistrationList;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  struct CrashDataThresholds {
    CrashDataThresholds(uint32 live_threads_threshold,
                        uint32 unresponsive_threshold);
    CrashDataThresholds();

    uint32 live_threads_threshold;
    uint32 unresponsive_threshold;
  };
  typedef std::map<std::string, CrashDataThresholds> CrashOnHangThreadMap;

  
  
  
  static void StartWatchingAll(const CommandLine& command_line);

  
  
  
  
  static void StopWatchingAll();

  
  static void Register(ThreadWatcher* watcher);

  
  static bool IsRegistered(const content::BrowserThread::ID thread_id);

  
  static void GetStatusOfThreads(uint32* responding_thread_count,
                                 uint32* unresponding_thread_count);

  
  
  static void WakeUpAll();

 private:
  
  friend class CustomThreadWatcher;
  friend class ThreadWatcherTest;
  FRIEND_TEST_ALL_PREFIXES(ThreadWatcherTest, ThreadNamesOnlyArgs);
  FRIEND_TEST_ALL_PREFIXES(ThreadWatcherTest, ThreadNamesAndLiveThresholdArgs);
  FRIEND_TEST_ALL_PREFIXES(ThreadWatcherTest, CrashOnHangThreadsAllArgs);

  
  ThreadWatcherList();

  
  virtual ~ThreadWatcherList();

  
  
  
  static void ParseCommandLine(
      const CommandLine& command_line,
      uint32* unresponsive_threshold,
      CrashOnHangThreadMap* crash_on_hang_threads);

  
  
  
  
  
  
  static void ParseCommandLineCrashOnHangThreads(
      const std::string& crash_on_hang_thread_names,
      uint32 default_live_threads_threshold,
      uint32 default_crash_seconds,
      CrashOnHangThreadMap* crash_on_hang_threads);

  
  
  
  static void InitializeAndStartWatching(
      uint32 unresponsive_threshold,
      const CrashOnHangThreadMap& crash_on_hang_threads);

  
  
  static void StartWatching(
      const content::BrowserThread::ID& thread_id,
      const std::string& thread_name,
      const base::TimeDelta& sleep_time,
      const base::TimeDelta& unresponsive_time,
      uint32 unresponsive_threshold,
      const CrashOnHangThreadMap& crash_on_hang_threads);

  
  
  static void DeleteAll();

  
  
  static ThreadWatcher* Find(const content::BrowserThread::ID& thread_id);

  
  
  static ThreadWatcherList* g_thread_watcher_list_;

  
  static const int kSleepSeconds;

  
  
  static const int kUnresponsiveSeconds;

  
  static const int kUnresponsiveCount;

  
  static const int kLiveThreadsThreshold;

  
  RegistrationList registered_;

  DISALLOW_COPY_AND_ASSIGN(ThreadWatcherList);
};

class ThreadWatcherObserver : public content::NotificationObserver {
 public:
  
  
  
  static void SetupNotifications(const base::TimeDelta& wakeup_interval);

  
  
  static void RemoveNotifications();

 private:
  
  explicit ThreadWatcherObserver(const base::TimeDelta& wakeup_interval);

  
  virtual ~ThreadWatcherObserver();

  
  
  
  
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  static ThreadWatcherObserver* g_thread_watcher_observer_;

  
  content::NotificationRegistrar registrar_;

  
  base::TimeTicks last_wakeup_time_;

  
  const base::TimeDelta wakeup_interval_;

  DISALLOW_COPY_AND_ASSIGN(ThreadWatcherObserver);
};

class WatchDogThread : public base::Thread {
 public:
  
  WatchDogThread();

  
  virtual ~WatchDogThread();

  
  
  static bool CurrentlyOnWatchDogThread();

  
  
  
  
  
  static bool PostTask(const tracked_objects::Location& from_here,
                       const base::Closure& task);
  static bool PostDelayedTask(const tracked_objects::Location& from_here,
                              const base::Closure& task,
                              base::TimeDelta delay);

 protected:
  virtual void Init() OVERRIDE;
  virtual void CleanUp() OVERRIDE;

 private:
  static bool PostTaskHelper(
      const tracked_objects::Location& from_here,
      const base::Closure& task,
      base::TimeDelta delay);

  DISALLOW_COPY_AND_ASSIGN(WatchDogThread);
};

class StartupTimeBomb {
 public:
  
  StartupTimeBomb();

  
  
  ~StartupTimeBomb();

  
  
  
  void Arm(const base::TimeDelta& duration);

  
  
  void Disarm();

  
  static void DisarmStartupTimeBomb();

 private:
  
  
  void DeleteStartupWatchdog();

  
  static StartupTimeBomb* g_startup_timebomb_;

  
  base::Watchdog* startup_watchdog_;

  
  const base::PlatformThreadId thread_id_;

  DISALLOW_COPY_AND_ASSIGN(StartupTimeBomb);
};

class ShutdownWatcherHelper {
 public:
  
  ShutdownWatcherHelper();

  
  ~ShutdownWatcherHelper();

  
  
  void Arm(const base::TimeDelta& duration);

 private:
  
  base::Watchdog* shutdown_watchdog_;

  
  const base::PlatformThreadId thread_id_;

  DISALLOW_COPY_AND_ASSIGN(ShutdownWatcherHelper);
};

#endif  
