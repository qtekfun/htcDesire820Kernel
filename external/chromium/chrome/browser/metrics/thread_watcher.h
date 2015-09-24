// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_METRICS_THREAD_WATCHER_H_
#define CHROME_BROWSER_METRICS_THREAD_WATCHER_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/metrics/histogram.h"
#include "base/synchronization/lock.h"
#include "base/task.h"
#include "base/threading/thread.h"
#include "base/time.h"
#include "content/browser/browser_thread.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class CustomThreadWatcher;
class ThreadWatcherList;

class ThreadWatcher {
 public:
  
  
  
  
  
  
  
  static void StartWatching(const BrowserThread::ID& thread_id,
                            const std::string& thread_name,
                            const base::TimeDelta& sleep_time,
                            const base::TimeDelta& unresponsive_time);

  
  BrowserThread::ID thread_id() const { return thread_id_; }

  
  std::string thread_name() const { return thread_name_; }

  
  base::TimeDelta sleep_time() const { return sleep_time_; }

  
  base::TimeDelta unresponsive_time() const { return unresponsive_time_; }

  
  bool active() const { return active_; }

  
  base::TimeTicks ping_time() const { return ping_time_; }

  
  uint64 ping_sequence_number() const { return ping_sequence_number_; }

 protected:
  
  
  
  ThreadWatcher(const BrowserThread::ID& thread_id,
                const std::string& thread_name,
                const base::TimeDelta& sleep_time,
                const base::TimeDelta& unresponsive_time);
  virtual ~ThreadWatcher();

  
  virtual void ActivateThreadWatching();

  
  virtual void DeActivateThreadWatching();

  
  
  
  virtual void WakeUp();

  
  
  
  
  
  
  virtual void PostPingMessage();

  
  
  
  
  
  virtual void OnPongMessage(uint64 ping_sequence_number);

  
  
  
  
  
  virtual bool OnCheckResponsiveness(uint64 ping_sequence_number);

 private:
  friend class ThreadWatcherList;

  
  FRIEND_TEST_ALL_PREFIXES(ThreadWatcherTest, Registration);
  FRIEND_TEST_ALL_PREFIXES(ThreadWatcherTest, ThreadResponding);
  FRIEND_TEST_ALL_PREFIXES(ThreadWatcherTest, ThreadNotResponding);
  FRIEND_TEST_ALL_PREFIXES(ThreadWatcherTest, MultipleThreadsResponding);
  FRIEND_TEST_ALL_PREFIXES(ThreadWatcherTest, MultipleThreadsNotResponding);

  
  void Initialize();

  
  
  static void OnPingMessage(const BrowserThread::ID& thread_id,
                            Task* callback_task);

  
  
  static const int kPingCount;

  
  
  const BrowserThread::ID thread_id_;

  
  const std::string thread_name_;

  
  
  const base::TimeDelta sleep_time_;

  
  
  const base::TimeDelta unresponsive_time_;

  
  base::TimeTicks ping_time_;

  
  
  
  uint64 ping_sequence_number_;

  
  bool active_;

  
  
  
  
  int ping_count_;

  
  base::Histogram* histogram_;

  
  
  
  ScopedRunnableMethodFactory<ThreadWatcher> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(ThreadWatcher);
};

class ThreadWatcherList : public NotificationObserver {
 public:
  
  typedef std::map<BrowserThread::ID, ThreadWatcher*> RegistrationList;

  
  ThreadWatcherList();
  
  virtual ~ThreadWatcherList();

  
  static void Register(ThreadWatcher* watcher);

  
  static bool IsRegistered(const BrowserThread::ID thread_id);

  
  
  
  static void StartWatchingAll();

  
  
  
  
  static void StopWatchingAll();

  
  
  static void RemoveNotifications();

 private:
  
  FRIEND_TEST_ALL_PREFIXES(ThreadWatcherTest, Registration);

  
  
  void DeleteAll();

  
  
  
  
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  virtual void WakeUpAll();

  
  
  static ThreadWatcher* Find(const BrowserThread::ID& thread_id);

  
  ThreadWatcher* PreLockedFind(const BrowserThread::ID& thread_id);

  static ThreadWatcherList* global_;  

  
  base::Lock lock_;

  
  RegistrationList registered_;

  
  NotificationRegistrar registrar_;

  
  base::TimeTicks last_wakeup_time_;

  DISALLOW_COPY_AND_ASSIGN(ThreadWatcherList);
};

class WatchDogThread : public base::Thread {
 public:
  
  WatchDogThread();

  
  virtual ~WatchDogThread();

  
  
  static bool CurrentlyOnWatchDogThread();

  
  
  
  
  
  static bool PostTask(const tracked_objects::Location& from_here, Task* task);
  static bool PostDelayedTask(const tracked_objects::Location& from_here,
                              Task* task,
                              int64 delay_ms);

 protected:
  virtual void Init();
  virtual void CleanUp();
  virtual void CleanUpAfterMessageLoopDestruction();

 private:
  static bool PostTaskHelper(
      const tracked_objects::Location& from_here,
      Task* task,
      int64 delay_ms);

  
  static base::Lock lock_;

  static WatchDogThread* watchdog_thread_;  

  DISALLOW_COPY_AND_ASSIGN(WatchDogThread);
};

DISABLE_RUNNABLE_METHOD_REFCOUNT(ThreadWatcher);
DISABLE_RUNNABLE_METHOD_REFCOUNT(ThreadWatcherList);

#endif  
