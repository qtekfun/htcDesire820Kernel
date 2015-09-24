// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_DEVICE_LOCAL_ACCOUNT_EXTERNAL_POLICY_LOADER_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_DEVICE_LOCAL_ACCOUNT_EXTERNAL_POLICY_LOADER_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/sequenced_task_runner.h"
#include "chrome/browser/chromeos/extensions/external_cache.h"
#include "chrome/browser/extensions/external_loader.h"
#include "components/policy/core/common/cloud/cloud_policy_store.h"

namespace chromeos {

class DeviceLocalAccountExternalPolicyLoader
    : public extensions::ExternalLoader,
      public policy::CloudPolicyStore::Observer,
      public ExternalCache::Delegate {
 public:
  
  
  DeviceLocalAccountExternalPolicyLoader(policy::CloudPolicyStore* store,
                                         const base::FilePath& cache_dir);

  
  
  bool IsCacheRunning() const;

  
  
  
  void StartCache(
      const scoped_refptr<base::SequencedTaskRunner>& cache_task_runner);

  
  
  void StopCache(const base::Closure& callback);

  
  virtual void StartLoading() OVERRIDE;

  
  virtual void OnStoreLoaded(policy::CloudPolicyStore* store) OVERRIDE;
  virtual void OnStoreError(policy::CloudPolicyStore* store) OVERRIDE;

  
  virtual void OnExtensionListsUpdated(
      const base::DictionaryValue* prefs) OVERRIDE;

 private:
  
  virtual ~DeviceLocalAccountExternalPolicyLoader();

  
  
  void UpdateExtensionListFromStore();

  policy::CloudPolicyStore* store_;
  const base::FilePath cache_dir_;
  scoped_ptr<ExternalCache> external_cache_;

  DISALLOW_COPY_AND_ASSIGN(DeviceLocalAccountExternalPolicyLoader);
};

}  

#endif  
