// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_EXTERNAL_DATA_MANAGER_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_EXTERNAL_DATA_MANAGER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "components/policy/core/common/external_data_manager.h"
#include "components/policy/policy_export.h"

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class CloudPolicyStore;

class POLICY_EXPORT CloudExternalDataManager : public ExternalDataManager {
 public:
  struct POLICY_EXPORT MetadataEntry {
    MetadataEntry();
    MetadataEntry(const std::string& url, const std::string& hash);

    bool operator!=(const MetadataEntry& other) const;

    std::string url;
    std::string hash;
  };
  
  
  typedef std::map<std::string, MetadataEntry> Metadata;

  CloudExternalDataManager();
  virtual ~CloudExternalDataManager();

  
  
  
  
  
  virtual void SetPolicyStore(CloudPolicyStore* policy_store);

  
  virtual void OnPolicyStoreLoaded() = 0;

  
  
  virtual void Connect(
      scoped_refptr<net::URLRequestContextGetter> request_context) = 0;

  
  
  virtual void Disconnect() = 0;

 protected:
  CloudPolicyStore* policy_store_;  

  base::WeakPtrFactory<CloudExternalDataManager> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(CloudExternalDataManager);
};

}  

#endif  
