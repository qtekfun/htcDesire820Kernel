// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_CLOUD_EXTERNAL_DATA_MANAGER_BASE_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_CLOUD_EXTERNAL_DATA_MANAGER_BASE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "components/policy/core/common/cloud/cloud_external_data_manager.h"
#include "components/policy/core/common/policy_details.h"

namespace base {
class SequencedTaskRunner;
}

namespace policy {

class CloudExternalDataStore;
class ExternalPolicyDataFetcherBackend;

class CloudExternalDataManagerBase : public CloudExternalDataManager,
                                     public base::NonThreadSafe {
 public:
  
  
  
  
  CloudExternalDataManagerBase(
      const GetChromePolicyDetailsCallback& get_policy_details,
      scoped_refptr<base::SequencedTaskRunner> backend_task_runner,
      scoped_refptr<base::SequencedTaskRunner> io_task_runner);
  virtual ~CloudExternalDataManagerBase();

  
  
  
  
  void SetExternalDataStore(
      scoped_ptr<CloudExternalDataStore> external_data_store);

  
  virtual void SetPolicyStore(CloudPolicyStore* policy_store) OVERRIDE;
  virtual void OnPolicyStoreLoaded() OVERRIDE;
  virtual void Connect(
      scoped_refptr<net::URLRequestContextGetter> request_context) OVERRIDE;
  virtual void Disconnect() OVERRIDE;
  virtual void Fetch(
      const std::string& policy,
      const ExternalDataFetcher::FetchCallback& callback) OVERRIDE;

  
  
  
  
  
  static void SetMaxExternalDataSizeForTesting(int max_size);

 protected:
  friend class CloudExternalDataManagerBaseTest;

  
  
  void FetchAll();

  scoped_refptr<base::SequencedTaskRunner> backend_task_runner_;
  scoped_refptr<base::SequencedTaskRunner> io_task_runner_;

 private:
  
  
  
  
  
  scoped_ptr<ExternalPolicyDataFetcherBackend>
      external_policy_data_fetcher_backend_;

  
  
  
  
  class Backend;
  scoped_ptr<Backend> backend_;

  DISALLOW_COPY_AND_ASSIGN(CloudExternalDataManagerBase);
};

}  

#endif  
