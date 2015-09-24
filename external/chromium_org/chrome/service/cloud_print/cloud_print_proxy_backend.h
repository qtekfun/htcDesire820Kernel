// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_SERVICE_CLOUD_PRINT_CLOUD_PRINT_PROXY_BACKEND_H_
#define CHROME_SERVICE_CLOUD_PRINT_CLOUD_PRINT_PROXY_BACKEND_H_

#include <list>
#include <string>

#include "base/threading/thread.h"
#include "chrome/service/cloud_print/connector_settings.h"
#include "printing/backend/print_backend.h"

namespace base {
class DictionaryValue;
}

namespace gaia {
struct OAuthClientInfo;
}

namespace cloud_print {

class CloudPrintProxyFrontend {
 public:
  CloudPrintProxyFrontend() {}

  
  
  virtual void OnAuthenticated(const std::string& robot_oauth_refresh_token,
                               const std::string& robot_email,
                               const std::string& user_email) = 0;
  
  virtual void OnAuthenticationFailed() = 0;
  
  virtual void OnPrintSystemUnavailable() = 0;
  
  virtual void OnUnregisterPrinters(
      const std::string& auth_token,
      const std::list<std::string>& printer_ids) = 0;
  
  virtual void OnXmppPingUpdated(int ping_timeout) = 0;

 protected:
  
  virtual ~CloudPrintProxyFrontend() {
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(CloudPrintProxyFrontend);
};

class CloudPrintProxyBackend {
 public:
  
  
  CloudPrintProxyBackend(CloudPrintProxyFrontend* frontend,
                         const ConnectorSettings& settings,
                         const gaia::OAuthClientInfo& oauth_client_info,
                         bool enable_job_poll);
  ~CloudPrintProxyBackend();

  
  
  bool InitializeWithToken(const std::string& cloud_print_token);
  
  bool InitializeWithRobotToken(const std::string& robot_oauth_refresh_token,
                                const std::string& robot_email);
  
  bool InitializeWithRobotAuthCode(const std::string& robot_oauth_auth_code,
                                   const std::string& robot_email);
  void Shutdown();
  void RegisterPrinters(const printing::PrinterList& printer_list);
  void UnregisterPrinters();

 private:
  
  class Core;
  
  
  base::Thread core_thread_;
  
  
  scoped_refptr<Core> core_;
  
  
  base::MessageLoop* const frontend_loop_;
  
  CloudPrintProxyFrontend* frontend_;

  friend class base::RefCountedThreadSafe<CloudPrintProxyBackend::Core>;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintProxyBackend);
};

}  

#endif  
