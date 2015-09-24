// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEMORY_DETAILS_H_
#define CHROME_BROWSER_MEMORY_DETAILS_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/process/process_metrics.h"
#include "base/strings/string16.h"
#include "chrome/browser/site_details.h"
#include "content/public/common/process_type.h"

struct ProcessMemoryInformation {
  
  
  enum RendererProcessType {
    RENDERER_UNKNOWN = 0,
    RENDERER_NORMAL,
    RENDERER_CHROME,        
    RENDERER_EXTENSION,     
    RENDERER_DEVTOOLS,      
    RENDERER_INTERSTITIAL,  
    RENDERER_NOTIFICATION,  
    RENDERER_BACKGROUND_APP 
  };

  static std::string GetRendererTypeNameInEnglish(RendererProcessType type);
  static std::string GetFullTypeNameInEnglish(
      int process_type,
      RendererProcessType rtype);

  ProcessMemoryInformation();
  ~ProcessMemoryInformation();

  
  bool operator<(const ProcessMemoryInformation& rhs) const;

  
  base::ProcessId pid;
  
  base::WorkingSetKBytes working_set;
  
  base::CommittedKBytes committed;
  
  base::string16 version;
  
  base::string16 product_name;
  
  int num_processes;
  
  
  
  bool is_diagnostics;
  
  int process_type;
  
  RendererProcessType renderer_type;
  
  std::vector<base::string16> titles;
};

typedef std::vector<ProcessMemoryInformation> ProcessMemoryInformationList;

struct ProcessData {
  ProcessData();
  ProcessData(const ProcessData& rhs);
  ~ProcessData();
  ProcessData& operator=(const ProcessData& rhs);

  base::string16 name;
  base::string16 process_name;
  ProcessMemoryInformationList processes;

  
  
  BrowserContextSiteDataMap site_data;
};

#if defined(OS_MACOSX)
class ProcessInfoSnapshot;
#endif

class MemoryDetails : public base::RefCountedThreadSafe<MemoryDetails> {
 public:
  enum UserMetricsMode {
    UPDATE_USER_METRICS,  
    SKIP_USER_METRICS
  };

  
  MemoryDetails();

  
  
  const std::vector<ProcessData>& processes() { return process_data_; }

  
  
  
  
  void StartFetch(UserMetricsMode user_metrics_mode);

  virtual void OnDetailsAvailable() = 0;

  
  
  std::string ToLogString();

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

#if defined(OS_CHROMEOS)
  void UpdateSwapHistograms();
#endif

  
  ProcessData* ChromeBrowser();

  std::vector<ProcessData> process_data_;

  UserMetricsMode user_metrics_mode_;

#if defined(OS_CHROMEOS)
  base::SwapInfo swap_info_;
#endif

  DISALLOW_COPY_AND_ASSIGN(MemoryDetails);
};

#endif  
