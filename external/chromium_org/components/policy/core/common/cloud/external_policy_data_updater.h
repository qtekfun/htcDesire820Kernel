// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_EXTERNAL_POLICY_DATA_UPDATER_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_EXTERNAL_POLICY_DATA_UPDATER_H_

#include <map>
#include <queue>
#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "components/policy/policy_export.h"

namespace base {
class SequencedTaskRunner;
}

namespace policy {

class ExternalPolicyDataFetcher;

class POLICY_EXPORT ExternalPolicyDataUpdater {
 public:
  struct POLICY_EXPORT Request {
   public:
    Request();
    Request(const std::string& url, const std::string& hash, int64 max_size);

    bool operator==(const Request& other) const;

    std::string url;
    std::string hash;
    int64 max_size;
  };

  
  
  
  
  
  
  
  
  
  
  
  typedef base::Callback<bool(const std::string&)> FetchSuccessCallback;

  
  
  
  ExternalPolicyDataUpdater(
      scoped_refptr<base::SequencedTaskRunner> task_runner,
      scoped_ptr<ExternalPolicyDataFetcher> external_policy_data_fetcher,
      size_t max_parallel_fetches);
  ~ExternalPolicyDataUpdater();

  
  
  
  
  
  void FetchExternalData(const std::string key,
                         const Request& request,
                         const FetchSuccessCallback& callback);

  
  
  void CancelExternalDataFetch(const std::string& key);

 private:
  class FetchJob;

  
  
  void StartNextJobs();

  
  
  void ScheduleJob(FetchJob* job);

  
  void OnJobSucceeded(FetchJob* job);

  
  void OnJobFailed(FetchJob* job);

  scoped_refptr<base::SequencedTaskRunner> task_runner_;
  scoped_ptr<ExternalPolicyDataFetcher> external_policy_data_fetcher_;

  
  size_t max_parallel_jobs_;

  
  size_t running_jobs_;

  
  
  std::queue<base::WeakPtr<FetchJob> > job_queue_;

  
  
  std::map<std::string, FetchJob*> job_map_;

  
  
  bool shutting_down_;

  DISALLOW_COPY_AND_ASSIGN(ExternalPolicyDataUpdater);
};

}  

#endif  
