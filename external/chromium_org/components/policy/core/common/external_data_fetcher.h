// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_EXTERNAL_DATA_FETCHER_H_
#define COMPONENTS_POLICY_CORE_COMMON_EXTERNAL_DATA_FETCHER_H_

#include <string>

#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "components/policy/policy_export.h"

namespace policy {

class ExternalDataManager;

class POLICY_EXPORT ExternalDataFetcher {
 public:
  typedef base::Callback<void(scoped_ptr<std::string>)> FetchCallback;

  
  
  ExternalDataFetcher(base::WeakPtr<ExternalDataManager> manager,
                      const std::string& policy);
  ExternalDataFetcher(const ExternalDataFetcher& other);

  ~ExternalDataFetcher();

  static bool Equals(const ExternalDataFetcher* first,
                     const ExternalDataFetcher* second);

  
  
  
  
  
  
  
  
  void Fetch(const FetchCallback& callback) const;

 private:
  base::WeakPtr<ExternalDataManager> manager_;
  const std::string policy_;
};

}  

#endif  
