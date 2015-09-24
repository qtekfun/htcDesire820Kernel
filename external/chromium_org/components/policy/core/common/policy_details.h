// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_POLICY_DETAILS_H_
#define COMPONENTS_POLICY_CORE_COMMON_POLICY_DETAILS_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "components/policy/policy_export.h"

namespace policy {

struct POLICY_EXPORT PolicyDetails {
  
  bool is_deprecated;

  
  bool is_device_policy;

  
  
  int id;

  
  
  
  size_t max_external_data_size;
};

typedef base::Callback<const PolicyDetails*(const std::string&)>
    GetChromePolicyDetailsCallback;

}  

#endif  
