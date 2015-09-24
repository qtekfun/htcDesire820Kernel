// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_POLICY_LOAD_STATUS_H_
#define COMPONENTS_POLICY_CORE_COMMON_POLICY_LOAD_STATUS_H_

#include <bitset>

#include "base/basictypes.h"
#include "components/policy/policy_export.h"

namespace base {
class HistogramBase;
}

namespace policy {

enum PolicyLoadStatus {
  
  
  
  POLICY_LOAD_STATUS_STARTED,
  
  POLICY_LOAD_STATUS_QUERY_FAILED,
  
  POLICY_LOAD_STATUS_NO_POLICY,
  
  POLICY_LOAD_STATUS_INACCCESSIBLE,
  
  POLICY_LOAD_STATUS_MISSING,
  
  POLICY_LOAD_STATUS_WOW64_REDIRECTION_DISABLED,
  
  POLICY_LOAD_STATUS_READ_ERROR,
  
  POLICY_LOAD_STATUS_TOO_BIG,
  
  POLICY_LOAD_STATUS_PARSE_ERROR,

  
  POLICY_LOAD_STATUS_SIZE
};

class POLICY_EXPORT PolicyLoadStatusSample {
 public:
  PolicyLoadStatusSample();
  ~PolicyLoadStatusSample();

  
  void Add(PolicyLoadStatus status);

 private:
  std::bitset<POLICY_LOAD_STATUS_SIZE> status_bits_;
  base::HistogramBase* histogram_;

  DISALLOW_COPY_AND_ASSIGN(PolicyLoadStatusSample);
};

}  

#endif  
