// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_CLOUD_PRINT_CLOUD_PRINT_PROXY_SERVICE_H_
#define CHROME_BROWSER_PRINTING_CLOUD_PRINT_CLOUD_PRINT_PROXY_SERVICE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_change_registrar.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

class Profile;
class ServiceProcessControl;

namespace base {
class DictionaryValue;
}  

namespace cloud_print {
struct CloudPrintProxyInfo;
}  

class CloudPrintProxyService : public BrowserContextKeyedService {
 public:
  explicit CloudPrintProxyService(Profile* profile);
  virtual ~CloudPrintProxyService();

  
  
  void Initialize();

  
  virtual void EnableForUserWithRobot(
      const std::string& robot_auth_code,
      const std::string& robot_email,
      const std::string& user_email,
      const base::DictionaryValue& user_settings);
  virtual void DisableForUser();

  
  
  void RefreshStatusFromService();

  
  
  
  bool EnforceCloudPrintConnectorPolicyAndQuit();

  std::string proxy_id() const { return proxy_id_; }

  
  static void GetPrintersAvalibleForRegistration(
      std::vector<std::string>* printers);

 private:
  
  class TokenExpiredNotificationDelegate;
  friend class TokenExpiredNotificationDelegate;

  
  void RefreshCloudPrintProxyStatus();
  void EnableCloudPrintProxyWithRobot(
      const std::string& robot_auth_code,
      const std::string& robot_email,
      const std::string& user_email,
      const base::DictionaryValue* user_preferences);
  void DisableCloudPrintProxy();

  
  void ProxyInfoCallback(
    const cloud_print::CloudPrintProxyInfo& proxy_info);

  
  
  
  bool InvokeServiceTask(const base::Closure& task);

  
  
  bool ApplyCloudPrintConnectorPolicy();

  Profile* profile_;
  std::string proxy_id_;

  
  virtual ServiceProcessControl* GetServiceProcessControl();

  base::WeakPtrFactory<CloudPrintProxyService> weak_factory_;

  
  PrefChangeRegistrar pref_change_registrar_;

  
  
  bool enforcing_connector_policy_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintProxyService);
};

#endif  
