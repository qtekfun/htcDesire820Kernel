// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_DEVICE_MANAGEMENT_SERVICE_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_DEVICE_MANAGEMENT_SERVICE_H_

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "components/policy/core/common/cloud/cloud_policy_constants.h"
#include "components/policy/policy_export.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "policy/proto/device_management_backend.pb.h"

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class DeviceManagementRequestJobImpl;
class DeviceManagementService;

class POLICY_EXPORT DeviceManagementRequestJob {
 public:
  
  enum JobType {
    TYPE_AUTO_ENROLLMENT,
    TYPE_REGISTRATION,
    TYPE_API_AUTH_CODE_FETCH,
    TYPE_POLICY_FETCH,
    TYPE_UNREGISTRATION,
    TYPE_UPLOAD_CERTIFICATE,
  };

  typedef base::Callback<
      void(DeviceManagementStatus, int,
           const enterprise_management::DeviceManagementResponse&)> Callback;

  typedef base::Callback<void(DeviceManagementRequestJob*)> RetryCallback;

  virtual ~DeviceManagementRequestJob();

  
  
  void SetGaiaToken(const std::string& gaia_token);
  void SetOAuthToken(const std::string& oauth_token);
  void SetUserAffiliation(UserAffiliation user_affiliation);
  void SetDMToken(const std::string& dm_token);
  void SetClientID(const std::string& client_id);
  enterprise_management::DeviceManagementRequest* GetRequest();

  
  
  
  
  
  void SetRetryCallback(const RetryCallback& retry_callback);

  
  void Start(const Callback& callback);

 protected:
  typedef std::vector<std::pair<std::string, std::string> > ParameterMap;

  DeviceManagementRequestJob(JobType type,
                             const std::string& agent_parameter,
                             const std::string& platform_parameter);

  
  void AddParameter(const std::string& name, const std::string& value);

  
  virtual void Run() = 0;

  ParameterMap query_params_;
  std::string gaia_token_;
  std::string dm_token_;
  enterprise_management::DeviceManagementRequest request_;
  RetryCallback retry_callback_;

  Callback callback_;

 private:
  DISALLOW_COPY_AND_ASSIGN(DeviceManagementRequestJob);
};

class POLICY_EXPORT DeviceManagementService : public net::URLFetcherDelegate {
 public:
  
  
  
  
  class POLICY_EXPORT Configuration {
   public:
    virtual ~Configuration() {}

    
    virtual std::string GetServerUrl() = 0;

    
    virtual std::string GetAgentParameter() = 0;

    
    virtual std::string GetPlatformParameter() = 0;
  };

  explicit DeviceManagementService(scoped_ptr<Configuration> configuration);
  virtual ~DeviceManagementService();

  
  
  
  static const int kURLFetcherID;

  
  
  virtual DeviceManagementRequestJob* CreateJob(
      DeviceManagementRequestJob::JobType type,
      net::URLRequestContextGetter* request_context);

  
  void ScheduleInitialization(int64 delay_milliseconds);

  
  void Shutdown();

  
  std::string GetServerUrl();

 private:
  typedef std::map<const net::URLFetcher*,
                   DeviceManagementRequestJobImpl*> JobFetcherMap;
  typedef std::deque<DeviceManagementRequestJobImpl*> JobQueue;

  friend class DeviceManagementRequestJobImpl;

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  void Initialize();

  
  void StartJob(DeviceManagementRequestJobImpl* job);

  
  
  void AddJob(DeviceManagementRequestJobImpl* job);

  
  
  void RemoveJob(DeviceManagementRequestJobImpl* job);

  
  
  scoped_ptr<Configuration> configuration_;

  
  JobFetcherMap pending_jobs_;

  
  JobQueue queued_jobs_;

  
  
  bool initialized_;

  
  base::WeakPtrFactory<DeviceManagementService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DeviceManagementService);
};

}  

#endif  
