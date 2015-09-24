// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_CLOUD_EXTERNAL_DATA_STORE_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_CLOUD_EXTERNAL_DATA_STORE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "components/policy/core/common/cloud/cloud_external_data_manager.h"

namespace base {
class SequencedTaskRunner;
}

namespace policy {

class ResourceCache;

class CloudExternalDataStore {
 public:
  CloudExternalDataStore(const std::string& cache_key,
                         scoped_refptr<base::SequencedTaskRunner> task_runner,
                         ResourceCache* cache);
  ~CloudExternalDataStore();

  
  
  void Prune(const CloudExternalDataManager::Metadata& metadata);

  
  bool Store(const std::string& policy,
             const std::string& hash,
             const std::string& data);

  
  
  
  
  bool Load(const std::string& policy,
            const std::string& hash,
            size_t max_size,
            std::string* data);

 private:
  std::string cache_key_;

  
  scoped_refptr<base::SequencedTaskRunner> task_runner_;

  ResourceCache* cache_;  

  DISALLOW_COPY_AND_ASSIGN(CloudExternalDataStore);
};

}  

#endif  
