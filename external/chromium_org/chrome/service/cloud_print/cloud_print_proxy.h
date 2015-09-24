// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_SERVICE_CLOUD_PRINT_CLOUD_PRINT_PROXY_H_
#define CHROME_SERVICE_CLOUD_PRINT_CLOUD_PRINT_PROXY_H_

#include <list>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/service/cloud_print/cloud_print_proxy_backend.h"
#include "chrome/service/cloud_print/cloud_print_wipeout.h"

class ServiceProcessPrefs;

namespace cloud_print {

struct CloudPrintProxyInfo;

class CloudPrintProxy : public CloudPrintProxyFrontend,
                        public CloudPrintWipeout::Client,
                        public base::NonThreadSafe {
 public:
  class Client {
   public:
    virtual ~Client() {}
    virtual void OnCloudPrintProxyEnabled(bool persist_state) {}
    virtual void OnCloudPrintProxyDisabled(bool persist_state) {}
  };
  CloudPrintProxy();
  virtual ~CloudPrintProxy();

  
  
  void Initialize(ServiceProcessPrefs* service_prefs, Client* client);

  
  void EnableForUser();
  void EnableForUserWithRobot(
      const std::string& robot_auth_code,
      const std::string& robot_email,
      const std::string& user_email,
      const base::DictionaryValue& user_settings);
  void UnregisterPrintersAndDisableForUser();
  void DisableForUser();
  
  void GetProxyInfo(CloudPrintProxyInfo* info);

  
  void CheckCloudPrintProxyPolicy();

  const std::string& user_email() const {
    return user_email_;
  }

  
  virtual void OnAuthenticated(const std::string& robot_oauth_refresh_token,
                               const std::string& robot_email,
                               const std::string& user_email) OVERRIDE;
  virtual void OnAuthenticationFailed() OVERRIDE;
  virtual void OnPrintSystemUnavailable() OVERRIDE;
  virtual void OnUnregisterPrinters(
      const std::string& auth_token,
      const std::list<std::string>& printer_ids) OVERRIDE;
  virtual void OnXmppPingUpdated(int ping_timeout) OVERRIDE;

  
  virtual void OnUnregisterPrintersComplete() OVERRIDE;

 protected:
  void ShutdownBackend();
  bool CreateBackend();

  
  
  scoped_ptr<CloudPrintProxyBackend> backend_;
  
  
  ServiceProcessPrefs* service_prefs_;
  
  
  Client* client_;
  
  
  std::string user_email_;
  
  
  bool enabled_;
  
  ConnectorSettings settings_;
  
  scoped_ptr<CloudPrintWipeout> wipeout_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintProxy);
};

}  

#endif  
