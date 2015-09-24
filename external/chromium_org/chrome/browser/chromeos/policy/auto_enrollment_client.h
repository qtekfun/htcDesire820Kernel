// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_AUTO_ENROLLMENT_CLIENT_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_AUTO_ENROLLMENT_CLIENT_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "components/policy/core/common/cloud/cloud_policy_constants.h"
#include "net/base/network_change_notifier.h"
#include "third_party/protobuf/src/google/protobuf/repeated_field.h"

class PrefRegistrySimple;
class PrefService;

namespace enterprise_management {
class DeviceManagementResponse;
}

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class DeviceManagementRequestJob;
class DeviceManagementService;

class AutoEnrollmentClient
    : public net::NetworkChangeNotifier::NetworkChangeObserver {
 public:
  
  
  
  
  
  AutoEnrollmentClient(
      const base::Closure& completion_callback,
      DeviceManagementService* device_management_service,
      PrefService* local_state,
      scoped_refptr<net::URLRequestContextGetter> system_request_context,
      const std::string& serial_number,
      int power_initial,
      int power_limit);
  virtual ~AutoEnrollmentClient();

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  
  static bool IsDisabled();

  
  static AutoEnrollmentClient* Create(const base::Closure& completion_callback);

  
  
  
  
  static void CancelAutoEnrollment();

  
  
  
  void Start();

  
  
  void CancelAndDeleteSoon();

  
  
  bool should_auto_enroll() const { return should_auto_enroll_; }

  
  
  std::string device_id() const { return device_id_; }

  
  virtual void OnNetworkChanged(
      net::NetworkChangeNotifier::ConnectionType type) OVERRIDE;

 private:
  
  
  bool GetCachedDecision();

  
  
  
  void SendRequest(int power);

  
  void OnRequestCompletion(
      DeviceManagementStatus status,
      int net_error,
      const enterprise_management::DeviceManagementResponse& response);

  
  bool IsSerialInProtobuf(
      const google::protobuf::RepeatedPtrField<std::string>& hashes);

  
  
  void OnProtocolDone();

  
  
  void OnRequestDone();

  
  base::Closure completion_callback_;

  
  
  bool should_auto_enroll_;

  
  std::string device_id_;

  
  
  std::string serial_number_hash_;

  
  
  int power_initial_;

  
  
  int power_limit_;

  
  
  int requests_sent_;

  
  DeviceManagementService* device_management_service_;
  scoped_ptr<DeviceManagementRequestJob> request_job_;

  
  PrefService* local_state_;

  
  scoped_refptr<net::URLRequestContextGetter> request_context_;

  
  
  
  
  
  base::Time time_start_;
  base::Time time_extra_start_;

  DISALLOW_COPY_AND_ASSIGN(AutoEnrollmentClient);
};

}  

#endif  
