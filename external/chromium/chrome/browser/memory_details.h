// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEMORY_DETAILS_H_
#define CHROME_BROWSER_MEMORY_DETAILS_H_
#pragma once

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/process_util.h"
#include "base/string16.h"
#include "content/common/child_process_info.h"

struct ProcessMemoryInformation {
  ProcessMemoryInformation();
  ~ProcessMemoryInformation();

  
  base::ProcessId pid;
  
  base::WorkingSetKBytes working_set;
  
  base::CommittedKBytes committed;
  
  string16 version;
  
  string16 product_name;
  
  int num_processes;
  
  
  
  bool is_diagnostics;
  
  ChildProcessInfo::ProcessType type;
  
  ChildProcessInfo::RendererProcessType renderer_type;
  
  std::vector<string16> titles;
};

typedef std::vector<ProcessMemoryInformation> ProcessMemoryInformationList;

struct ProcessData {
  ProcessData();
  ProcessData(const ProcessData& rhs);
  ~ProcessData();
  ProcessData& operator=(const ProcessData& rhs);

  string16 name;
  string16 process_name;
  ProcessMemoryInformationList processes;
};

#if defined(OS_MACOSX)
class ProcessInfoSnapshot;
#endif

class MemoryDetails : public base::RefCountedThreadSafe<MemoryDetails> {
 public:
  
  MemoryDetails();

  
  
  const std::vector<ProcessData>& processes() { return process_data_; }

  
  
  
  void StartFetch();

  virtual void OnDetailsAvailable() {}

 protected:
  friend class base::RefCountedThreadSafe<MemoryDetails>;

  virtual ~MemoryDetails();

 private:
  
  
  
  
  
  void CollectChildInfoOnIOThread();

  
  
  
  
  
  
  void CollectProcessData(const std::vector<ProcessMemoryInformation>&);

#if defined(OS_MACOSX)
  
  
  
  void CollectProcessDataChrome(
      const std::vector<ProcessMemoryInformation>& child_info,
      base::ProcessId pid,
      const ProcessInfoSnapshot& process_info);
#endif

  
  
  void CollectChildInfoOnUIThread();

  
  
  void UpdateHistograms();

  
  ProcessData* ChromeBrowser();

  std::vector<ProcessData> process_data_;

  DISALLOW_COPY_AND_ASSIGN(MemoryDetails);
};

#endif  
