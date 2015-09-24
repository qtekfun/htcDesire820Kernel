// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_CHROME_PROCESS_UTIL_H_
#define CHROME_TEST_BASE_CHROME_PROCESS_UTIL_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/process/process_handle.h"
#include "base/process/process_metrics.h"

typedef std::vector<base::ProcessId> ChromeProcessList;

ChromeProcessList GetRunningChromeProcesses(base::ProcessId browser_pid);

void TerminateAllChromeProcesses(const ChromeProcessList& process_list);

class ChromeTestProcessMetrics {
 public:
  static ChromeTestProcessMetrics* CreateProcessMetrics(
        base::ProcessHandle process) {
    return new ChromeTestProcessMetrics(process);
  }

  size_t GetPagefileUsage();

  size_t GetWorkingSetSize();

  size_t GetPeakPagefileUsage() {
    return process_metrics_->GetPeakPagefileUsage();
  }

  size_t GetPeakWorkingSetSize() {
    return process_metrics_->GetPeakWorkingSetSize();
  }

  bool GetIOCounters(base::IoCounters* io_counters) {
    return process_metrics_->GetIOCounters(io_counters);
  }

  base::ProcessHandle process_handle_;

  ~ChromeTestProcessMetrics();

 private:
  explicit ChromeTestProcessMetrics(base::ProcessHandle process);

  scoped_ptr<base::ProcessMetrics> process_metrics_;

  DISALLOW_COPY_AND_ASSIGN(ChromeTestProcessMetrics);
};

#if defined(OS_MACOSX)


struct MacChromeProcessInfo {
  base::ProcessId pid;
  int rsz_in_kb;
  int vsz_in_kb;
};

typedef std::vector<MacChromeProcessInfo> MacChromeProcessInfoList;

MacChromeProcessInfoList GetRunningMacProcessInfo(
                                        const ChromeProcessList& process_list);

#endif  

#endif  
