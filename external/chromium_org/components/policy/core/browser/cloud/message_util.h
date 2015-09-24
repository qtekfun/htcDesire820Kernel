// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_BROWSER_CLOUD_MESSAGE_UTIL_H_
#define COMPONENTS_POLICY_CORE_BROWSER_CLOUD_MESSAGE_UTIL_H_

#include "base/strings/string16.h"
#include "components/policy/core/common/cloud/cloud_policy_constants.h"
#include "components/policy/core/common/cloud/cloud_policy_store.h"
#include "components/policy/core/common/cloud/cloud_policy_validator.h"
#include "components/policy/policy_export.h"

namespace policy {

POLICY_EXPORT string16 FormatDeviceManagementStatus(
    DeviceManagementStatus status);

POLICY_EXPORT string16 FormatValidationStatus(
    CloudPolicyValidatorBase::Status validation_status);

POLICY_EXPORT string16 FormatStoreStatus(
    CloudPolicyStore::Status store_status,
    CloudPolicyValidatorBase::Status validation_status);

}  

#endif  
