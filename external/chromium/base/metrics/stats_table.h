// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_METRICS_STATS_TABLE_H_
#define BASE_METRICS_STATS_TABLE_H_
#pragma once

#include <string>

#include "base/base_api.h"
#include "base/basictypes.h"
#include "base/hash_tables.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_local_storage.h"

namespace base {

class BASE_API StatsTable {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  StatsTable(const std::string& name, int max_threads, int max_counters);

  
  
  ~StatsTable();

  
  
  static StatsTable* current() { return global_table_; }

  
  static void set_current(StatsTable* value) { global_table_ = value; }

  
  
  int GetSlot() const;

  
  
  
  
  
  
  
  
  
  
  
  int RegisterThread(const std::string& name);

  
  
  int CountThreadsRegistered() const;

  
  
  
  
  
  
  int FindCounter(const std::string& name);

  

  
  
  int* GetLocation(int counter_id, int slot_id) const;

  
  
  const char* GetRowName(int index) const;

  
  int GetRowValue(int index) const;

  
  int GetRowValue(int index, int pid) const;

  
  
  int GetCounterValue(const std::string& name);

  
  
  int GetCounterValue(const std::string& name, int pid);

  
  int GetMaxCounters() const;

  
  int GetMaxThreads() const;

  
  
  static const int kMaxThreadNameLength = 32;

  
  
  static const int kMaxCounterNameLength = 64;

  
  
  
  static int* FindLocation(const char *name);

 private:
  class Private;
  struct TLSData;
  typedef hash_map<std::string, int> CountersMap;

  
  
  
  
  void UnregisterThread();

  
  
  void UnregisterThread(TLSData* tls_data);

  
  
  static void SlotReturnFunction(void* data);

  
  
  
  int FindEmptyThread() const;

  
  
  
  int FindCounterOrEmptyRow(const std::string& name) const;

  
  
  
  
  
  
  
  
  int AddCounter(const std::string& name);

  
  
  TLSData* GetTLSData() const;

  Private* impl_;

  
  base::Lock counters_lock_;

  
  
  
  
  
  CountersMap counters_;
  ThreadLocalStorage::Slot tls_index_;

  static StatsTable* global_table_;

  DISALLOW_COPY_AND_ASSIGN(StatsTable);
};

}  

#endif  
