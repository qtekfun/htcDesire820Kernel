// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_INTERNET_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_INTERNET_OPTIONS_HANDLER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "chromeos/login/login_state.h"
#include "chromeos/network/network_state_handler_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/gfx/native_widget_types.h"

class Browser;

namespace chromeos {
class DeviceState;
class NetworkState;
class NetworkStateHandlerObserver;
}

namespace gfx {
class ImageSkia;
}

namespace views {
class WidgetDelegate;
}

namespace chromeos {
namespace options {

class InternetOptionsHandler
    : public ::options::OptionsPageUIHandler,
      public chromeos::NetworkStateHandlerObserver,
      public chromeos::LoginState::Observer {
 public:
  InternetOptionsHandler();
  virtual ~InternetOptionsHandler();

 private:
  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;
  virtual void InitializePage() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

  
  void EnableWifiCallback(const base::ListValue* args);
  void DisableWifiCallback(const base::ListValue* args);
  void EnableCellularCallback(const base::ListValue* args);
  void DisableCellularCallback(const base::ListValue* args);
  void EnableWimaxCallback(const base::ListValue* args);
  void DisableWimaxCallback(const base::ListValue* args);
  void ShowMorePlanInfoCallback(const base::ListValue* args);
  void BuyDataPlanCallback(const base::ListValue* args);
  void SetApnCallback(const base::ListValue* args);
  void SetApnProperties(const base::ListValue* args,
                        const std::string& service_path,
                        const base::DictionaryValue& shill_properties);
  void CarrierStatusCallback();
  void SetCarrierCallback(const base::ListValue* args);
  void SetSimCardLockCallback(const base::ListValue* args);
  void ChangePinCallback(const base::ListValue* args);
  void RefreshNetworksCallback(const base::ListValue* args);

  
  std::string GetIconDataUrl(int resource_id) const;

  
  void RefreshNetworkData();

  
  
  void UpdateConnectionData(const std::string& service_path);
  void UpdateConnectionDataCallback(
      const std::string& service_path,
      const base::DictionaryValue& shill_properties);
  
  void UpdateCarrier();

  
  virtual void DeviceListChanged() OVERRIDE;
  virtual void NetworkListChanged() OVERRIDE;
  virtual void NetworkConnectionStateChanged(
      const chromeos::NetworkState* network) OVERRIDE;
  virtual void NetworkPropertiesUpdated(
      const chromeos::NetworkState* network) OVERRIDE;

  
  virtual void LoggedInStateChanged() OVERRIDE;

  
  void UpdateLoggedInUserType();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void SetServerHostnameCallback(const base::ListValue* args);
  void SetPreferNetworkCallback(const base::ListValue* args);
  void SetAutoConnectCallback(const base::ListValue* args);
  void SetIPConfigCallback(const base::ListValue* args);
  void SetIPConfigProperties(const base::ListValue* args,
                             const std::string& service_path,
                             const base::DictionaryValue& shill_properties);

  
  
  void PopulateDictionaryDetailsCallback(
      const std::string& service_path,
      const base::DictionaryValue& shill_properties);

  
  gfx::NativeWindow GetNativeWindow() const;

  
  
  
  
  void NetworkCommandCallback(const base::ListValue* args);

  
  void AddConnection(const std::string& type);

  
  base::ListValue* GetWiredList();

  
  base::ListValue* GetWirelessList();

  
  base::ListValue* GetVPNList();

  
  base::ListValue* GetRememberedList();

  
  void FillNetworkInfo(base::DictionaryValue* dictionary);

  content::NotificationRegistrar registrar_;

  
  std::string details_path_;

  
  
  
  base::WeakPtrFactory<InternetOptionsHandler> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(InternetOptionsHandler);
};

}  
}  

#endif  
