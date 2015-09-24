// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_ENTERPRISE_METRICS_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_ENTERPRISE_METRICS_H_

#include "components/policy/policy_export.h"

namespace policy {


enum MetricToken {
  
  kMetricTokenLoadSucceeded,
  
  kMetricTokenLoadFailed,

  
  kMetricTokenFetchRequested,
  
  kMetricTokenFetchRequestFailed,
  
  kMetricTokenFetchServerFailed,
  
  kMetricTokenFetchResponseReceived,
  
  
  kMetricTokenFetchBadResponse,
  
  kMetricTokenFetchManagementNotSupported,
  
  kMetricTokenFetchDeviceNotFound,
  
  kMetricTokenFetchOK,

  
  kMetricTokenStoreSucceeded,
  
  kMetricTokenStoreFailed,

  
  kMetricTokenFetchDeviceIdConflict,
  
  kMetricTokenFetchInvalidSerialNumber,
  // DM server reported that the licenses for the domain have expired or been
  
  kMetricMissingLicenses,

  kMetricTokenSize  
};

enum MetricPolicy {
  
  kMetricPolicyLoadSucceeded,
  
  kMetricPolicyLoadFailed,

  
  kMetricPolicyFetchRequested,
  
  kMetricPolicyFetchRequestFailed,
  
  kMetricPolicyFetchServerFailed,
  
  kMetricPolicyFetchNotFound,
  
  kMetricPolicyFetchInvalidToken,
  
  kMetricPolicyFetchResponseReceived,
  
  
  kMetricPolicyFetchBadResponse,
  
  kMetricPolicyFetchInvalidPolicy,
  
  kMetricPolicyFetchBadSignature,
  
  kMetricPolicyFetchTimestampInFuture,
  
  kMetricPolicyFetchNonEnterpriseDevice,
  
  
  kMetricPolicyFetchUserMismatch,
  
  
  
  kMetricPolicyFetchOtherFailed,
  
  kMetricPolicyFetchOK,
  
  
  kMetricPolicyFetchNotModified,

  
  kMetricPolicyStoreSucceeded,
  
  kMetricPolicyStoreFailed,

  kMetricPolicySize  
};

enum MetricEnrollment {
  
  
  kMetricEnrollmentCancelled,
  
  kMetricEnrollmentStarted,
  
  kMetricEnrollmentNetworkFailed,
  
  kMetricEnrollmentLoginFailed,
  
  kMetricEnrollmentNotSupported,
  
  kMetricEnrollmentPolicyFailed,
  
  
  
  
  kMetricEnrollmentOtherFailed,
  
  kMetricEnrollmentOK,
  
  
  kMetricEnrollmentInvalidSerialNumber,
  
  kMetricEnrollmentAutoStarted,
  
  kMetricEnrollmentAutoFailed,
  
  kMetricEnrollmentAutoRetried,
  
  kMetricEnrollmentAutoCancelled,
  
  kMetricEnrollmentAutoOK,
  
  
  kMetricEnrollmentInvalidEnrollmentMode,
  
  
  kMetricEnrollmentAutoEnrollmentNotSupported,
  
  
  kMetricLockboxTimeoutError,
  
  
  kMetricEnrollmentWrongUserError,
  // DM server reported that the licenses for the domain has expired or been
  
  kMetricMissingLicensesError,
  
  
  kMetricEnrollmentRobotAuthCodeFetchFailed,
  
  
  kMetricEnrollmentRobotRefreshTokenFetchFailed,
  
  
  kMetricEnrollmentRobotRefreshTokenStoreFailed,
  
  kMetricEnrollmentDeprovisioned,

  kMetricEnrollmentSize  
};

enum MetricPolicyRefresh {
  
  
  METRIC_POLICY_REFRESH_CHANGED,
  
  
  METRIC_POLICY_REFRESH_CHANGED_NO_INVALIDATIONS,
  
  
  METRIC_POLICY_REFRESH_UNCHANGED,
  
  
  METRIC_POLICY_REFRESH_INVALIDATED_CHANGED,
  
  
  METRIC_POLICY_REFRESH_INVALIDATED_UNCHANGED,

  METRIC_POLICY_REFRESH_SIZE  
};

POLICY_EXPORT extern const char kMetricToken[];
POLICY_EXPORT extern const char kMetricPolicy[];
POLICY_EXPORT extern const char kMetricEnrollment[];
POLICY_EXPORT extern const char kMetricPolicyRefresh[];
POLICY_EXPORT extern const char kMetricPolicyInvalidations[];

}  

#endif  
