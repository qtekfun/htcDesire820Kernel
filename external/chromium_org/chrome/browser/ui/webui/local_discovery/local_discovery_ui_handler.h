// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_LOCAL_DISCOVERY_LOCAL_DISCOVERY_UI_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_LOCAL_DISCOVERY_LOCAL_DISCOVERY_UI_HANDLER_H_

#include <map>
#include <string>
#include <vector>

#include "base/cancelable_callback.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/local_discovery/cloud_print_printer_list.h"
#include "chrome/browser/local_discovery/privet_device_lister.h"
#include "chrome/browser/local_discovery/privet_http.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_ui_message_handler.h"

#if defined(ENABLE_FULL_PRINTING) && !defined(OS_CHROMEOS) && \
  !defined(OS_MACOSX)
#define CLOUD_PRINT_CONNECTOR_UI_AVAILABLE
#endif

namespace local_discovery {

class PrivetConfirmApiCallFlow;
class PrivetHTTPAsynchronousFactory;
class PrivetHTTPResolution;
class ServiceDiscoverySharedClient;

class LocalDiscoveryUIHandler : public content::WebUIMessageHandler,
                                public PrivetRegisterOperation::Delegate,
                                public PrivetDeviceLister::Delegate,
                                public CloudPrintPrinterList::Delegate,
                                content::NotificationObserver {
 public:
  LocalDiscoveryUIHandler();
  virtual ~LocalDiscoveryUIHandler();

  static bool GetHasVisible();

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void OnPrivetRegisterClaimToken(
      PrivetRegisterOperation* operation,
      const std::string& token,
      const GURL& url) OVERRIDE;

  virtual void OnPrivetRegisterError(
      PrivetRegisterOperation* operation,
      const std::string& action,
      PrivetRegisterOperation::FailureReason reason,
      int printer_http_code,
      const DictionaryValue* json) OVERRIDE;

  virtual void OnPrivetRegisterDone(
      PrivetRegisterOperation* operation,
      const std::string& device_id) OVERRIDE;

  
  virtual void DeviceChanged(bool added,
                             const std::string& name,
                             const DeviceDescription& description) OVERRIDE;

  virtual void DeviceRemoved(const std::string& name) OVERRIDE;

  virtual void DeviceCacheFlushed() OVERRIDE;

  
  virtual void OnCloudPrintPrinterListReady() OVERRIDE;

  virtual void OnCloudPrintPrinterListUnavailable() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  typedef std::map<std::string, DeviceDescription> DeviceDescriptionMap;

  
  
  void HandleStart(const base::ListValue* args);

  
  void HandleIsVisible(const base::ListValue* args);

  
  void HandleRegisterDevice(const base::ListValue* args);

  
  void HandleCancelRegistration(const base::ListValue* args);

  
  void HandleRequestPrinterList(const base::ListValue* args);

  
  
  void HandleOpenCloudPrintURL(const base::ListValue* args);

  
  void HandleShowSyncUI(const base::ListValue* args);

  
  void StartRegisterHTTP(
      scoped_ptr<PrivetHTTPClient> http_client);

  
  
  void OnConfirmDone(CloudPrintBaseApiFlow::Status status);

  
  void SendRegisterError();

  
  void SendRegisterDone(const std::string& service_name,
                        const DeviceDescription& device);

  
  void SetIsVisible(bool visible);

  
  std::string GetSyncAccount();

  
  std::string GetCloudPrintBaseUrl();

  
  void ResetCurrentRegistration();

  scoped_ptr<base::DictionaryValue> CreatePrinterInfo(
      const CloudPrintPrinterList::PrinterDetails& description);

  
  
  
  void OnAnnouncementTimeoutReached();

  void CheckUserLoggedIn();

#if defined(CLOUD_PRINT_CONNECTOR_UI_AVAILABLE)
  void StartCloudPrintConnector();
  void OnCloudPrintPrefsChanged();
  void ShowCloudPrintSetupDialog(const ListValue* args);
  void HandleDisableCloudPrintConnector(const ListValue* args);
  void SetupCloudPrintConnectorSection();
  void RemoveCloudPrintConnectorSection();
  void RefreshCloudPrintStatusFromService();
#endif

  
  scoped_ptr<PrivetHTTPClient> current_http_client_;

  
  scoped_ptr<PrivetRegisterOperation> current_register_operation_;

  
  scoped_ptr<PrivetConfirmApiCallFlow> confirm_api_call_flow_;

  
  scoped_ptr<PrivetDeviceLister> privet_lister_;

  
  scoped_refptr<ServiceDiscoverySharedClient> service_discovery_client_;

  
  scoped_ptr<PrivetHTTPAsynchronousFactory> privet_http_factory_;

  
  scoped_ptr<PrivetHTTPResolution> privet_resolution_;

  
  DeviceDescriptionMap device_descriptions_;

  
  bool is_visible_;

  
  scoped_ptr<CloudPrintPrinterList> cloud_print_printer_list_;

#if defined(CLOUD_PRINT_CONNECTOR_UI_AVAILABLE)
  StringPrefMember cloud_print_connector_email_;
  BooleanPrefMember cloud_print_connector_enabled_;
  bool cloud_print_connector_ui_enabled_;
#endif

  content::NotificationRegistrar notification_registrar_;
  DISALLOW_COPY_AND_ASSIGN(LocalDiscoveryUIHandler);
};

#undef CLOUD_PRINT_CONNECTOR_UI_AVAILABLE

}  
#endif  
