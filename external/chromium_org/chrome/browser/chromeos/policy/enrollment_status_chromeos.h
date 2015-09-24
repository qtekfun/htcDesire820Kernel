// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_ENROLLMENT_STATUS_CHROMEOS_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_ENROLLMENT_STATUS_CHROMEOS_H_

#include "components/policy/core/common/cloud/cloud_policy_constants.h"
#include "components/policy/core/common/cloud/cloud_policy_store.h"
#include "components/policy/core/common/cloud/cloud_policy_validator.h"

namespace policy {

class EnrollmentStatus {
 public:
  
  enum Status {
    STATUS_SUCCESS,                     
    STATUS_REGISTRATION_FAILED,         
    STATUS_REGISTRATION_BAD_MODE,       
    STATUS_ROBOT_AUTH_FETCH_FAILED,     
    STATUS_ROBOT_REFRESH_FETCH_FAILED,  
    STATUS_ROBOT_REFRESH_STORE_FAILED,  
    STATUS_POLICY_FETCH_FAILED,         
    STATUS_VALIDATION_FAILED,           
    STATUS_LOCK_ERROR,                  
    STATUS_LOCK_TIMEOUT,                
    STATUS_LOCK_WRONG_USER,             
    STATUS_STORE_ERROR,                 
  };

  
  static EnrollmentStatus ForStatus(Status status);
  static EnrollmentStatus ForRegistrationError(
      DeviceManagementStatus client_status);
  static EnrollmentStatus ForFetchError(DeviceManagementStatus client_status);
  static EnrollmentStatus ForRobotAuthFetchError(
      DeviceManagementStatus client_status);
  static EnrollmentStatus ForRobotRefreshFetchError(int http_status);
  static EnrollmentStatus ForValidationError(
      CloudPolicyValidatorBase::Status validation_status);
  static EnrollmentStatus ForStoreError(
      CloudPolicyStore::Status store_error,
      CloudPolicyValidatorBase::Status validation_status);

  Status status() const { return status_; }
  DeviceManagementStatus client_status() const { return client_status_; }
  int http_status() const { return http_status_; }
  CloudPolicyStore::Status store_status() const { return store_status_; }
  CloudPolicyValidatorBase::Status validation_status() const {
    return validation_status_;
  }

 private:
  EnrollmentStatus(Status status,
                   DeviceManagementStatus client_status,
                   int http_status,
                   CloudPolicyStore::Status store_status,
                   CloudPolicyValidatorBase::Status validation_status);

  Status status_;
  DeviceManagementStatus client_status_;
  int http_status_;
  CloudPolicyStore::Status store_status_;
  CloudPolicyValidatorBase::Status validation_status_;
};

}  

#endif  
