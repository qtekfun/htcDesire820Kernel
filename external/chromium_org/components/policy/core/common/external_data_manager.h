// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_EXTERNAL_DATA_MANAGER_H_
#define COMPONENTS_POLICY_CORE_COMMON_EXTERNAL_DATA_MANAGER_H_

#include <string>

#include "components/policy/core/common/external_data_fetcher.h"
#include "components/policy/policy_export.h"

namespace policy {

class POLICY_EXPORT ExternalDataManager {
 public:
  
  
  
  
  
  
  
  
  virtual void Fetch(const std::string& policy,
                     const ExternalDataFetcher::FetchCallback& callback) = 0;
};

}  

#endif  
