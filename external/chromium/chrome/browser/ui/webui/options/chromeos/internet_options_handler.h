// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_INTERNET_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_INTERNET_OPTIONS_HANDLER_H_

#include <string>

#include "chrome/browser/chromeos/cros/network_library.h"
#include "chrome/browser/ui/webui/options/chromeos/cros_options_page_ui_handler.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/native_widget_types.h"

class SkBitmap;
namespace views {
class WindowDelegate;
}

class InternetOptionsHandler
  : public chromeos::CrosOptionsPageUIHandler,
    public chromeos::NetworkLibrary::NetworkManagerObserver,
    public chromeos::NetworkLibrary::NetworkObserver,
    public chromeos::NetworkLibrary::CellularDataPlanObserver {
 public:
  InternetOptionsHandler();
  virtual ~InternetOptionsHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);
  virtual void Initialize();

  
  virtual void RegisterMessages();

  
  virtual void OnNetworkManagerChanged(chromeos::NetworkLibrary* network_lib);
  
  virtual void OnNetworkChanged(chromeos::NetworkLibrary* network_lib,
                                const chromeos::Network* network);
  
  virtual void OnCellularDataPlanChanged(chromeos::NetworkLibrary* network_lib);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

 private:
  
  void CreateModalPopup(views::WindowDelegate* view);
  gfx::NativeWindow GetNativeWindow() const;

  
  
  
  
  
  
  void ButtonClickCallback(const ListValue* args);
  void HandleWifiButtonClick(const std::string& service_path,
                             const std::string& command);
  void HandleCellularButtonClick(const std::string& service_path,
                                 const std::string& command);
  void HandleVPNButtonClick(const std::string& service_path,
                            const std::string& command);

  
  
  
  void RefreshCellularPlanCallback(const ListValue* args);
  void SetActivationButtonVisibility(
      const chromeos::CellularNetwork* cellular,
      DictionaryValue* dictionary);

  void LoginCallback(const ListValue* args);
  void LoginCertCallback(const ListValue* args);
  void LoginToOtherCallback(const ListValue* args);
  void SetDetailsCallback(const ListValue* args);
  void EnableWifiCallback(const ListValue* args);
  void DisableWifiCallback(const ListValue* args);
  void EnableCellularCallback(const ListValue* args);
  void DisableCellularCallback(const ListValue* args);
  void BuyDataPlanCallback(const ListValue* args);
  void SetApnCallback(const ListValue* args);
  void SetSimCardLockCallback(const ListValue* args);
  void ChangePinCallback(const ListValue* args);

  
  
  void PopulateDictionaryDetails(const chromeos::Network* net,
                                 chromeos::NetworkLibrary* cros);
  void PopulateWifiDetails(const chromeos::WifiNetwork* wifi,
                           DictionaryValue* dictionary);
  void PopulateCellularDetails(chromeos::NetworkLibrary* cros,
                               const chromeos::CellularNetwork* cellular,
                               DictionaryValue* dictionary);
  void PopulateVPNDetails(const chromeos::VirtualNetwork* vpn,
                          DictionaryValue* dictionary);

  
  
  DictionaryValue* CellularDataPlanToDictionary(
      const chromeos::CellularDataPlan* plan);
  
  ListValue* GetNetwork(const std::string& service_path,
                        const SkBitmap& icon,
                        const std::string& name,
                        bool connecting,
                        bool connected,
                        bool connectable,
                        chromeos::ConnectionType connection_type,
                        bool remembered,
                        chromeos::ActivationState activation_state,
                        bool restricted_ip);

  
  ListValue* GetWiredList();
  
  ListValue* GetWirelessList();
  
  ListValue* GetRememberedList();
  
  void FillNetworkInfo(DictionaryValue* dictionary,
                       chromeos::NetworkLibrary* cros);
  
  void RefreshNetworkData(chromeos::NetworkLibrary* cros);
  
  
  
  void MonitorNetworks(chromeos::NetworkLibrary* cros);

  
  
  bool use_settings_ui_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(InternetOptionsHandler);
};

#endif  
