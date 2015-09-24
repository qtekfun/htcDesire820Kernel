// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_PROCESS_PROCESS_METRICS_H_
#define BASE_PROCESS_PROCESS_METRICS_H_

#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/process/process_handle.h"
#include "base/time/time.h"
#include "base/values.h"

#if defined(OS_MACOSX)
#include <mach/mach.h>
#endif

namespace base {

#if defined(OS_WIN)
struct IoCounters : public IO_COUNTERS {
};
#elif defined(OS_POSIX)
struct IoCounters {
  uint64_t ReadOperationCount;
  uint64_t WriteOperationCount;
  uint64_t OtherOperationCount;
  uint64_t ReadTransferCount;
  uint64_t WriteTransferCount;
  uint64_t OtherTransferCount;
};
#endif


struct WorkingSetKBytes {
  WorkingSetKBytes() : priv(0), shareable(0), shared(0) {}
  size_t priv;
  size_t shareable;
  size_t shared;
#if defined(OS_CHROMEOS)
  size_t swapped;
#endif
};

struct CommittedKBytes {
  CommittedKBytes() : priv(0), mapped(0), image(0) {}
  size_t priv;
  size_t mapped;
  size_t image;
};

struct FreeMBytes {
  size_t total;
  size_t largest;
  void* largest_ptr;
};

BASE_EXPORT int64 TimeValToMicroseconds(const struct timeval& tv);

class BASE_EXPORT ProcessMetrics {
 public:
  ~ProcessMetrics();

  
  
#if !defined(OS_MACOSX) || defined(OS_IOS)
  static ProcessMetrics* CreateProcessMetrics(ProcessHandle process);
#else
  class PortProvider {
   public:
    virtual ~PortProvider() {}

    
    
    
    
    virtual mach_port_t TaskForPid(ProcessHandle process) const = 0;
  };

  
  
  
  static ProcessMetrics* CreateProcessMetrics(ProcessHandle process,
                                              PortProvider* port_provider);
#endif  

  
  
  
  size_t GetPagefileUsage() const;
  
  size_t GetPeakPagefileUsage() const;
  
  
  size_t GetWorkingSetSize() const;
  
  size_t GetPeakWorkingSetSize() const;
  
  
  
  
  bool GetMemoryBytes(size_t* private_bytes,
                      size_t* shared_bytes);
  
  
  void GetCommittedKBytes(CommittedKBytes* usage) const;
  
  
  bool GetWorkingSetKBytes(WorkingSetKBytes* ws_usage) const;

  
  
  
  
  bool CalculateFreeMemory(FreeMBytes* free) const;

  
  
  
  
  
  
  double GetCPUUsage();

  
  
  
  double GetPlatformIndependentCPUUsage();

  
  
  
  
  
  bool GetIOCounters(IoCounters* io_counters) const;

 private:
#if !defined(OS_MACOSX) || defined(OS_IOS)
  explicit ProcessMetrics(ProcessHandle process);
#else
  ProcessMetrics(ProcessHandle process, PortProvider* port_provider);
#endif  

#if defined(OS_LINUX) || defined(OS_ANDROID)
  bool GetWorkingSetKBytesStatm(WorkingSetKBytes* ws_usage) const;
#endif

#if defined(OS_CHROMEOS)
  bool GetWorkingSetKBytesTotmaps(WorkingSetKBytes *ws_usage) const;
#endif

  ProcessHandle process_;

  int processor_count_;

  
  
  int64 last_time_;
  int64 last_system_time_;

#if !defined(OS_IOS)
#if defined(OS_MACOSX)
  
  mach_port_t TaskForPid(ProcessHandle process) const;

  PortProvider* port_provider_;
#elif defined(OS_POSIX)
  
  int last_cpu_;
#endif  
#endif  

  DISALLOW_COPY_AND_ASSIGN(ProcessMetrics);
};

BASE_EXPORT size_t GetSystemCommitCharge();

#if defined(OS_POSIX)
size_t GetMaxFds();
#endif  

#if defined(OS_LINUX) || defined(OS_ANDROID)
BASE_EXPORT int ParseProcStatCPU(const std::string& input);

BASE_EXPORT int GetNumberOfThreads(ProcessHandle process);

BASE_EXPORT extern const char kProcSelfExe[];

struct BASE_EXPORT SystemMemoryInfoKB {
  SystemMemoryInfoKB();

  
  scoped_ptr<Value> ToValue() const;

  int total;
  int free;
  int buffers;
  int cached;
  int active_anon;
  int inactive_anon;
  int active_file;
  int inactive_file;
  int swap_total;
  int swap_free;
  int dirty;

  
  int pswpin;
  int pswpout;
  int pgmajfault;

#ifdef OS_CHROMEOS
  int shmem;
  int slab;
  
  int gem_objects;
  long long gem_size;
#endif
};

BASE_EXPORT bool ParseProcMeminfo(const std::string& input,
                                  SystemMemoryInfoKB* meminfo);

BASE_EXPORT bool ParseProcVmstat(const std::string& input,
                                 SystemMemoryInfoKB* meminfo);

BASE_EXPORT bool GetSystemMemoryInfo(SystemMemoryInfoKB* meminfo);

struct BASE_EXPORT SystemDiskInfo {
  SystemDiskInfo();

  
  scoped_ptr<Value> ToValue() const;

  uint64 reads;
  uint64 reads_merged;
  uint64 sectors_read;
  uint64 read_time;
  uint64 writes;
  uint64 writes_merged;
  uint64 sectors_written;
  uint64 write_time;
  uint64 io;
  uint64 io_time;
  uint64 weighted_io_time;
};

BASE_EXPORT bool IsValidDiskName(const std::string& candidate);

BASE_EXPORT bool GetSystemDiskInfo(SystemDiskInfo* diskinfo);
#endif  

#if defined(OS_CHROMEOS)
struct BASE_EXPORT SwapInfo {
  SwapInfo()
      : num_reads(0),
        num_writes(0),
        compr_data_size(0),
        orig_data_size(0),
        mem_used_total(0) {
  }

  
  scoped_ptr<Value> ToValue() const;

  uint64 num_reads;
  uint64 num_writes;
  uint64 compr_data_size;
  uint64 orig_data_size;
  uint64 mem_used_total;
};

BASE_EXPORT void GetSwapInfo(SwapInfo* swap_info);
#endif  

class SystemMetrics {
 public:
  SystemMetrics();

  static SystemMetrics Sample();

  
  scoped_ptr<Value> ToValue() const;

 private:
  FRIEND_TEST_ALL_PREFIXES(SystemMetricsTest, SystemMetrics);

  size_t committed_memory_;
#if defined(OS_LINUX) || defined(OS_ANDROID)
  SystemMemoryInfoKB memory_info_;
  SystemDiskInfo disk_info_;
#endif
#if defined(OS_CHROMEOS)
  SwapInfo swap_info_;
#endif
};

}  

#endif  
