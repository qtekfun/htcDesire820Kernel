// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_DEVICE_MANAGEMENT_SERVICE_H_
#define CHROME_BROWSER_POLICY_DEVICE_MANAGEMENT_SERVICE_H_
#pragma once

#include <deque>
#include <map>
#include <string>

#include "base/basictypes.h"
#include "chrome/browser/policy/device_management_backend.h"
#include "chrome/common/net/url_fetcher.h"
#include "googleurl/src/gurl.h"

class Profile;

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class DeviceManagementBackendImpl;

class DeviceManagementService : public URLFetcher::Delegate {
 public:
  
  class DeviceManagementJob {
   public:
    virtual ~DeviceManagementJob() {}

    
    virtual void HandleResponse(const net::URLRequestStatus& status,
                                int response_code,
                                const ResponseCookies& cookies,
                                const std::string& data) = 0;

    
    virtual GURL GetURL(const std::string& server_url) = 0;

    
    virtual void ConfigureRequest(URLFetcher* fetcher) = 0;
  };

  explicit DeviceManagementService(const std::string& server_url);
  virtual ~DeviceManagementService();

  
  
  
  virtual DeviceManagementBackend* CreateBackend();

  
  
  void Initialize(net::URLRequestContextGetter* request_context_getter);

  
  
  void Shutdown();

  
  
  void AddJob(DeviceManagementJob* job);

  
  
  void RemoveJob(DeviceManagementJob* job);

 private:
  typedef std::map<const URLFetcher*, DeviceManagementJob*> JobFetcherMap;
  typedef std::deque<DeviceManagementJob*> JobQueue;

  
  void StartJob(DeviceManagementJob* job);

  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  
  const std::string server_url_;

  
  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;

  
  JobFetcherMap pending_jobs_;

  
  JobQueue queued_jobs_;

  DISALLOW_COPY_AND_ASSIGN(DeviceManagementService);
};

}  

#endif  
