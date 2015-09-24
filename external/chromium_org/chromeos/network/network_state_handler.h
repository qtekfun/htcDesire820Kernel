// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_NETWORK_STATE_HANDLER_H_
#define CHROMEOS_NETWORK_NETWORK_STATE_HANDLER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/network/managed_state.h"
#include "chromeos/network/network_handler.h"
#include "chromeos/network/network_handler_callbacks.h"
#include "chromeos/network/shill_property_handler.h"

namespace base {
class DictionaryValue;
class ListValue;
class Value;
}

namespace tracked_objects {
class Location;
}

namespace chromeos {

class DeviceState;
class NetworkState;
class NetworkStateHandlerObserver;
class NetworkStateHandlerTest;
class NetworkTypePattern;


class CHROMEOS_EXPORT NetworkStateHandler
    : public internal::ShillPropertyHandler::Listener {
 public:
  typedef std::vector<ManagedState*> ManagedStateList;
  typedef std::vector<const NetworkState*> NetworkStateList;
  typedef std::vector<const DeviceState*> DeviceStateList;
  typedef std::vector<const FavoriteState*> FavoriteStateList;

  enum TechnologyState {
    TECHNOLOGY_UNAVAILABLE,
    TECHNOLOGY_AVAILABLE,
    TECHNOLOGY_UNINITIALIZED,
    TECHNOLOGY_ENABLING,
    TECHNOLOGY_ENABLED
  };

  virtual ~NetworkStateHandler();

  
  void AddObserver(NetworkStateHandlerObserver* observer,
                   const tracked_objects::Location& from_here);
  void RemoveObserver(NetworkStateHandlerObserver* observer,
                      const tracked_objects::Location& from_here);

  
  
  
  void UpdateManagerProperties();

  
  
  TechnologyState GetTechnologyState(const NetworkTypePattern& type) const;
  bool IsTechnologyAvailable(const NetworkTypePattern& type) const {
    return GetTechnologyState(type) != TECHNOLOGY_UNAVAILABLE;
  }
  bool IsTechnologyEnabled(const NetworkTypePattern& type) const {
    return GetTechnologyState(type) == TECHNOLOGY_ENABLED;
  }

  
  
  
  void SetTechnologyEnabled(
      const NetworkTypePattern& type,
      bool enabled,
      const network_handler::ErrorCallback& error_callback);

  
  const DeviceState* GetDeviceState(const std::string& device_path) const;

  
  const DeviceState* GetDeviceStateByType(const NetworkTypePattern& type) const;

  
  bool GetScanningByType(const NetworkTypePattern& type) const;

  
  
  
  
  
  const NetworkState* GetNetworkState(const std::string& service_path) const;

  
  
  const NetworkState* DefaultNetwork() const;

  
  
  const FavoriteState* DefaultFavoriteNetwork() const;

  
  const NetworkState* ConnectedNetworkByType(
      const NetworkTypePattern& type) const;

  
  const NetworkState* ConnectingNetworkByType(
      const NetworkTypePattern& type) const;

  
  
  
  const NetworkState* FirstNetworkByType(const NetworkTypePattern& type) const;

  
  
  std::string FormattedHardwareAddressForType(
      const NetworkTypePattern& type) const;

  
  
  
  
  void GetNetworkList(NetworkStateList* list) const;

  
  void GetNetworkListByType(const NetworkTypePattern& type,
                            NetworkStateList* list) const;

  
  
  
  
  void GetDeviceList(DeviceStateList* list) const;

  
  void GetDeviceListByType(const NetworkTypePattern& type,
                           DeviceStateList* list) const;

  
  
  
  
  
  void GetFavoriteList(FavoriteStateList* list) const;

  
  void GetFavoriteListByType(const NetworkTypePattern& type,
                             FavoriteStateList* list) const;

  
  const FavoriteState* GetFavoriteState(const std::string& service_path) const;

  
  
  void RequestScan() const;

  
  
  void WaitForScan(const std::string& type, const base::Closure& callback);

  
  
  void ConnectToBestWifiNetwork();

  
  
  
  
  
  
  
  void RequestUpdateForNetwork(const std::string& service_path);

  
  
  
  void RequestUpdateForAllNetworks();

  
  void SetCheckPortalList(const std::string& check_portal_list);

  const std::string& GetCheckPortalListForTest() const {
    return check_portal_list_;
  }

  
  
  
  
  const FavoriteState* GetEAPForEthernet(const std::string& service_path) const;

  
  
  void GetNetworkStatePropertiesForTest(
      base::DictionaryValue* dictionary) const;

  
  static NetworkStateHandler* InitializeForTest();

  
  
  static const char kDefaultCheckPortalList[];

 protected:
  friend class NetworkHandler;
  NetworkStateHandler();

  

  
  
  virtual void UpdateManagedList(ManagedState::ManagedType type,
                                 const base::ListValue& entries) OVERRIDE;

  
  
  virtual void ProfileListChanged() OVERRIDE;

  
  
  virtual void UpdateManagedStateProperties(
      ManagedState::ManagedType type,
      const std::string& path,
      const base::DictionaryValue& properties) OVERRIDE;

  
  virtual void UpdateNetworkServiceProperty(
      const std::string& service_path,
      const std::string& key,
      const base::Value& value) OVERRIDE;

  
  virtual void UpdateDeviceProperty(
      const std::string& device_path,
      const std::string& key,
      const base::Value& value) OVERRIDE;

  
  
  virtual void CheckPortalListChanged(
      const std::string& check_portal_list) OVERRIDE;

  
  virtual void TechnologyListChanged() OVERRIDE;

  
  
  
  
  virtual void ManagedStateListChanged(
      ManagedState::ManagedType type) OVERRIDE;

  
  
  void InitShillPropertyHandler();

 private:
  typedef std::list<base::Closure> ScanCallbackList;
  typedef std::map<std::string, ScanCallbackList> ScanCompleteCallbackMap;
  friend class NetworkStateHandlerTest;
  FRIEND_TEST_ALL_PREFIXES(NetworkStateHandlerTest, NetworkStateHandlerStub);

  
  
  void UpdateNetworkStateProperties(NetworkState* network,
                                    const base::DictionaryValue& properties);

  
  void NotifyDeviceListChanged();

  
  
  
  DeviceState* GetModifiableDeviceState(const std::string& device_path) const;
  NetworkState* GetModifiableNetworkState(
      const std::string& service_path) const;
  ManagedState* GetModifiableManagedState(const ManagedStateList* managed_list,
                                          const std::string& path) const;

  
  ManagedStateList* GetManagedList(ManagedState::ManagedType type);

  
  void OnNetworkConnectionStateChanged(NetworkState* network);

  
  
  bool CheckDefaultNetworkChanged();

  
  void OnDefaultNetworkChanged();

  
  void NetworkPropertiesUpdated(const NetworkState* network);

  
  void ScanCompleted(const std::string& type);

  
  std::string GetTechnologyForType(const NetworkTypePattern& type) const;

  
  scoped_ptr<internal::ShillPropertyHandler> shill_property_handler_;

  
  ObserverList<NetworkStateHandlerObserver> observers_;

  
  ManagedStateList network_list_;

  
  
  
  ManagedStateList favorite_list_;

  
  ManagedStateList device_list_;

  
  std::string default_network_path_;

  
  std::string check_portal_list_;

  
  ScanCompleteCallbackMap scan_complete_callbacks_;

  DISALLOW_COPY_AND_ASSIGN(NetworkStateHandler);
};

}  

#endif  
