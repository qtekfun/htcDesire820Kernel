// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TRACKED_OBJECTS_H_
#define BASE_TRACKED_OBJECTS_H_

#include <map>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/lazy_instance.h"
#include "base/location.h"
#include "base/profiler/alternate_timer.h"
#include "base/profiler/tracked_time.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_local_storage.h"

namespace base {
struct TrackingInfo;
}


namespace tracked_objects {

class ThreadData;
class BASE_EXPORT BirthOnThread {
 public:
  BirthOnThread(const Location& location, const ThreadData& current);

  const Location location() const { return location_; }
  const ThreadData* birth_thread() const { return birth_thread_; }

 private:
  
  
  
  const Location location_;

  
  
  const ThreadData* const birth_thread_;

  DISALLOW_COPY_AND_ASSIGN(BirthOnThread);
};


struct BASE_EXPORT BirthOnThreadSnapshot {
  BirthOnThreadSnapshot();
  explicit BirthOnThreadSnapshot(const BirthOnThread& birth);
  ~BirthOnThreadSnapshot();

  LocationSnapshot location;
  std::string thread_name;
};


class BASE_EXPORT Births: public BirthOnThread {
 public:
  Births(const Location& location, const ThreadData& current);

  int birth_count() const;

  
  void RecordBirth();

  
  
  void ForgetBirth();

  
  void Clear();

 private:
  
  int birth_count_;

  DISALLOW_COPY_AND_ASSIGN(Births);
};


class BASE_EXPORT DeathData {
 public:
  
  DeathData();

  
  
  
  explicit DeathData(int count);

  
  
  void RecordDeath(const int32 queue_duration,
                   const int32 run_duration,
                   int random_number);

  
  int count() const;
  int32 run_duration_sum() const;
  int32 run_duration_max() const;
  int32 run_duration_sample() const;
  int32 queue_duration_sum() const;
  int32 queue_duration_max() const;
  int32 queue_duration_sample() const;

  
  void ResetMax();

  
  void Clear();

 private:
  
  
  
  int count_;
  
  int32 run_duration_sum_;
  int32 queue_duration_sum_;
  
  
  int32 run_duration_max_;
  int32 queue_duration_max_;
  
  
  int32 run_duration_sample_;
  int32 queue_duration_sample_;
};


struct BASE_EXPORT DeathDataSnapshot {
  DeathDataSnapshot();
  explicit DeathDataSnapshot(const DeathData& death_data);
  ~DeathDataSnapshot();

  int count;
  int32 run_duration_sum;
  int32 run_duration_max;
  int32 run_duration_sample;
  int32 queue_duration_sum;
  int32 queue_duration_max;
  int32 queue_duration_sample;
};


struct BASE_EXPORT TaskSnapshot {
  TaskSnapshot();
  TaskSnapshot(const BirthOnThread& birth,
               const DeathData& death_data,
               const std::string& death_thread_name);
  ~TaskSnapshot();

  BirthOnThreadSnapshot birth;
  DeathDataSnapshot death_data;
  std::string death_thread_name;
};


struct ProcessDataSnapshot;
class BASE_EXPORT ThreadData {
 public:
  
  
  enum Status {
    UNINITIALIZED,              
    DORMANT_DURING_TESTS,       
    DEACTIVATED,                
    PROFILING_ACTIVE,           
    PROFILING_CHILDREN_ACTIVE,  
  };

  typedef std::map<Location, Births*> BirthMap;
  typedef std::map<const Births*, DeathData> DeathMap;
  typedef std::pair<const Births*, const Births*> ParentChildPair;
  typedef std::set<ParentChildPair> ParentChildSet;
  typedef std::stack<const Births*> ParentStack;

  
  
  
  
  static void InitializeThreadContext(const std::string& suggested_name);

  
  
  
  
  static ThreadData* Get();

  
  
  
  static void Snapshot(bool reset_max, ProcessDataSnapshot* process_data);

  
  
  
  static Births* TallyABirthIfActive(const Location& location);

  
  
  
  
  
  
  
  
  static void TallyRunOnNamedThreadIfTracking(
      const base::TrackingInfo& completed_task,
      const TrackedTime& start_of_run,
      const TrackedTime& end_of_run);

  
  
  
  
  
  
  
  static void TallyRunOnWorkerThreadIfTracking(
      const Births* birth,
      const TrackedTime& time_posted,
      const TrackedTime& start_of_run,
      const TrackedTime& end_of_run);

  
  
  static void TallyRunInAScopedRegionIfTracking(
      const Births* birth,
      const TrackedTime& start_of_run,
      const TrackedTime& end_of_run);

  const std::string& thread_name() const { return thread_name_; }

  
  
  
  
  static void ResetAllThreadData();

  
  
  static bool Initialize();

  
  
  
  
  
  
  
  
  
  
  static bool InitializeAndSetTrackingStatus(Status status);

  static Status status();

  
  
  static bool TrackingStatus();

  
  
  static bool TrackingParentChildStatus();

  
  
  
  
  
  
  static TrackedTime NowForStartOfRun(const Births* parent);
  static TrackedTime NowForEndOfRun();

  
  
  
  
  static TrackedTime Now();

  
  
  
  
  static void SetAlternateTimeSource(NowFunction* now);

  
  
  
  static void EnsureCleanupWasCalled(int major_threads_shutdown_count);

 private:
  
  
  
  
  
  friend class TrackedObjectsTest;
  FRIEND_TEST_ALL_PREFIXES(TrackedObjectsTest, MinimalStartupShutdown);
  FRIEND_TEST_ALL_PREFIXES(TrackedObjectsTest, TinyStartupShutdown);
  FRIEND_TEST_ALL_PREFIXES(TrackedObjectsTest, ParentChildTest);

  typedef std::map<const BirthOnThread*, int> BirthCountMap;

  
  explicit ThreadData(int thread_number);

  
  explicit ThreadData(const std::string& suggested_name);

  ~ThreadData();

  
  
  
  void PushToHeadOfList();

  
  static ThreadData* first();

  
  ThreadData* next() const;


  
  Births* TallyABirth(const Location& location);

  
  void TallyADeath(const Births& birth, int32 queue_duration, int32 duration);

  
  
  
  
  
  static void SnapshotAllExecutedTasks(bool reset_max,
                                       ProcessDataSnapshot* process_data,
                                       BirthCountMap* birth_counts);

  
  
  
  
  
  
  
  
  void SnapshotExecutedTasks(bool reset_max,
                             ProcessDataSnapshot* process_data,
                             BirthCountMap* birth_counts);

  
  
  
  
  
  void SnapshotMaps(bool reset_max,
                    BirthMap* birth_map,
                    DeathMap* death_map,
                    ParentChildSet* parent_child_set);

  
  void Reset();

  
  
  static void OnThreadTermination(void* thread_data);

  
  
  void OnThreadTerminationCleanup();

  
  
  
  
  
  
  
  
  
  static void ShutdownSingleThreadedCleanup(bool leak);

  
  
  static NowFunction* now_function_;

  
  static base::ThreadLocalStorage::StaticSlot tls_index_;

  
  
  
  
  
  static ThreadData* first_retired_worker_;

  
  
  
  static ThreadData* all_thread_data_list_head_;

  
  
  static int worker_thread_data_creation_count_;

  
  
  static int cleanup_count_;

  
  
  
  static int incarnation_counter_;

  
  
  
  
  static base::LazyInstance<base::Lock>::Leaky list_lock_;

  
  static Status status_;

  
  
  
  ThreadData* next_;

  
  
  
  ThreadData* next_retired_worker_;

  
  
  std::string thread_name_;

  
  
  
  
  int worker_thread_number_;

  
  
  
  
  BirthMap birth_map_;

  
  
  
  
  DeathMap death_map_;

  
  
  
  ParentChildSet parent_child_set_;

  
  // regularly read and written on this thread, but may only be read from other
  
  
  
  
  mutable base::Lock map_lock_;

  
  
  
  
  
  
  
  
  ParentStack parent_stack_;

  
  
  
  
  int32 random_number_;

  
  
  
  
  int incarnation_count_for_pool_;

  DISALLOW_COPY_AND_ASSIGN(ThreadData);
};


struct BASE_EXPORT ParentChildPairSnapshot {
 public:
  ParentChildPairSnapshot();
  explicit ParentChildPairSnapshot(
      const ThreadData::ParentChildPair& parent_child);
  ~ParentChildPairSnapshot();

  BirthOnThreadSnapshot parent;
  BirthOnThreadSnapshot child;
};


struct BASE_EXPORT ProcessDataSnapshot {
 public:
  ProcessDataSnapshot();
  ~ProcessDataSnapshot();

  std::vector<TaskSnapshot> tasks;
  std::vector<ParentChildPairSnapshot> descendants;
  int process_id;
};

}  

#endif  
