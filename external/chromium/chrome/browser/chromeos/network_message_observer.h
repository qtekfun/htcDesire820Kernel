// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_NETWORK_MESSAGE_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_NETWORK_MESSAGE_OBSERVER_H_
#pragma once

#include <map>
#include <string>

#include "base/basictypes.h"
#include "chrome/browser/chromeos/cros/network_library.h"
#include "chrome/browser/chromeos/notifications/system_notification.h"

class Profile;

namespace chromeos {


class NetworkMessageObserver : public NetworkLibrary::NetworkManagerObserver,
                               public NetworkLibrary::CellularDataPlanObserver,
                               public NetworkLibrary::UserActionObserver {
 public:
  explicit NetworkMessageObserver(Profile* profile);
  virtual ~NetworkMessageObserver();

  static bool IsApplicableBackupPlan(const CellularDataPlan* plan,
                                     const CellularDataPlan* other_plan);
 private:
  virtual void OpenMobileSetupPage(const ListValue* args);
  virtual void OpenMoreInfoPage(const ListValue* args);
  virtual void InitNewPlan(const CellularDataPlan* plan);
  virtual void ShowNeedsPlanNotification(const CellularNetwork* cellular);
  virtual void ShowNoDataNotification(CellularDataPlanType plan_type);
  virtual void ShowLowDataNotification(const CellularDataPlan* plan);
  virtual bool CheckNetworkFailed(const Network* network);

  
  virtual void OnNetworkManagerChanged(NetworkLibrary* obj);
  
  virtual void OnCellularDataPlanChanged(NetworkLibrary* obj);
  
  virtual void OnConnectionInitiated(NetworkLibrary* obj,
                                     const Network* network);

  
  
  void SaveLastCellularInfo(const CellularNetwork* cellular,
                            const CellularDataPlan* plan);

  typedef std::map<std::string, ConnectionState> NetworkStateMap;

  
  NetworkStateMap network_states_;

  
  std::string cellular_service_path_;
  
  std::string cellular_data_plan_unique_id_;
  
  CellularDataPlanType cellular_data_plan_type_;
  
  CellularNetwork::DataLeft cellular_data_left_;

  
  SystemNotification notification_connection_error_;
  
  SystemNotification notification_low_data_;
  
  SystemNotification notification_no_data_;

  DISALLOW_COPY_AND_ASSIGN(NetworkMessageObserver);
};

}  

#endif  
