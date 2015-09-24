// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_CLIENT_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_CLIENT_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "components/policy/core/common/cloud/cloud_policy_constants.h"
#include "components/policy/policy_export.h"
#include "policy/proto/device_management_backend.pb.h"

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class DeviceManagementRequestJob;
class DeviceManagementService;

class POLICY_EXPORT CloudPolicyClient {
 public:
  
  typedef std::map<PolicyNamespaceKey,
                   enterprise_management::PolicyFetchResponse*> ResponseMap;

  
  
  typedef base::Callback<void(bool status)> StatusCallback;

  
  class POLICY_EXPORT Observer {
   public:
    virtual ~Observer();

    
    
    virtual void OnPolicyFetched(CloudPolicyClient* client) = 0;

    
    
    virtual void OnRegistrationStateChanged(CloudPolicyClient* client) = 0;

    
    
    
    virtual void OnRobotAuthCodesFetched(CloudPolicyClient* client);

    
    virtual void OnClientError(CloudPolicyClient* client) = 0;
  };

  
  
  class POLICY_EXPORT StatusProvider {
   public:
    virtual ~StatusProvider();

    
    
    virtual bool GetDeviceStatus(
        enterprise_management::DeviceStatusReportRequest* status) = 0;
    virtual bool GetSessionStatus(
        enterprise_management::SessionStatusReportRequest* status) = 0;

    
    
    virtual void OnSubmittedSuccessfully() = 0;
  };

  
  
  CloudPolicyClient(
      const std::string& machine_id,
      const std::string& machine_model,
      UserAffiliation user_affiliation,
      StatusProvider* provider,
      DeviceManagementService* service,
      scoped_refptr<net::URLRequestContextGetter> request_context);
  virtual ~CloudPolicyClient();

  
  
  
  virtual void SetupRegistration(const std::string& dm_token,
                                 const std::string& client_id);

  
  
  virtual void Register(
      enterprise_management::DeviceRegisterRequest::Type registration_type,
      const std::string& auth_token,
      const std::string& client_id,
      bool is_auto_enrollment,
      const std::string& requisition);

  
  
  
  void SetInvalidationInfo(int64 version, const std::string& payload);

  
  
  
  
  
  
  
  virtual void FetchPolicy();

  
  
  
  virtual void FetchRobotAuthCodes(const std::string& auth_token);

  
  virtual void Unregister();

  
  
  
  
  virtual void UploadCertificate(const std::string& certificate_data,
                                 const StatusCallback& callback);

  
  void AddObserver(Observer* observer);

  
  void RemoveObserver(Observer* observer);

  void set_submit_machine_id(bool submit_machine_id) {
    submit_machine_id_ = submit_machine_id;
  }

  void set_last_policy_timestamp(const base::Time& timestamp) {
    last_policy_timestamp_ = timestamp;
  }

  void set_public_key_version(int public_key_version) {
    public_key_version_ = public_key_version;
    public_key_version_valid_ = true;
  }

  void clear_public_key_version() {
    public_key_version_valid_ = false;
  }

  
  void AddNamespaceToFetch(const PolicyNamespaceKey& policy_ns_key);

  
  void RemoveNamespaceToFetch(const PolicyNamespaceKey& policy_ns_key);

  
  bool is_registered() const { return !dm_token_.empty(); }

  const std::string& dm_token() const { return dm_token_; }
  const std::string& client_id() const { return client_id_; }

  
  DeviceMode device_mode() const { return device_mode_; }

  
  
  
  
  const ResponseMap& responses() const {
    return responses_;
  }

  
  
  const enterprise_management::PolicyFetchResponse* GetPolicyFor(
      const PolicyNamespaceKey& policy_ns_key) const;

  DeviceManagementStatus status() const {
    return status_;
  }

  const std::string& robot_api_auth_code() const {
    return robot_api_auth_code_;
  }

  
  
  
  int64 fetched_invalidation_version() const {
    return fetched_invalidation_version_;
  }

  scoped_refptr<net::URLRequestContextGetter> GetRequestContext();

 protected:
  
  typedef std::set<PolicyNamespaceKey> NamespaceSet;

  
  void OnRetryRegister(DeviceManagementRequestJob* job);

  
  void OnRegisterCompleted(
      DeviceManagementStatus status,
      int net_error,
      const enterprise_management::DeviceManagementResponse& response);

  
  void OnPolicyFetchCompleted(
      DeviceManagementStatus status,
      int net_error,
      const enterprise_management::DeviceManagementResponse& response);

  
  void OnFetchRobotAuthCodesCompleted(
      DeviceManagementStatus status,
      int net_error,
      const enterprise_management::DeviceManagementResponse& response);

  
  void OnUnregisterCompleted(
      DeviceManagementStatus status,
      int net_error,
      const enterprise_management::DeviceManagementResponse& response);

  
  void OnCertificateUploadCompleted(
      const StatusCallback& callback,
      DeviceManagementStatus status,
      int net_error,
      const enterprise_management::DeviceManagementResponse& response);

  
  void NotifyPolicyFetched();
  void NotifyRegistrationStateChanged();
  void NotifyRobotAuthCodesFetched();
  void NotifyClientError();

  
  const std::string machine_id_;
  const std::string machine_model_;
  const UserAffiliation user_affiliation_;
  NamespaceSet namespaces_to_fetch_;

  std::string dm_token_;
  DeviceMode device_mode_;
  std::string client_id_;
  bool submit_machine_id_;
  base::Time last_policy_timestamp_;
  int public_key_version_;
  bool public_key_version_valid_;
  std::string robot_api_auth_code_;

  
  int64 invalidation_version_;
  std::string invalidation_payload_;

  
  int64 fetched_invalidation_version_;

  
  DeviceManagementService* service_;
  scoped_ptr<DeviceManagementRequestJob> request_job_;

  
  StatusProvider* status_provider_;

  
  ResponseMap responses_;
  DeviceManagementStatus status_;

  ObserverList<Observer, true> observers_;
  scoped_refptr<net::URLRequestContextGetter> request_context_;

 private:
  DISALLOW_COPY_AND_ASSIGN(CloudPolicyClient);
};

}  

#endif  
