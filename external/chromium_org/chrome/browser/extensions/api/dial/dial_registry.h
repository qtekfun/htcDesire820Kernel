// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DIAL_DIAL_REGISTRY_H_
#define CHROME_BROWSER_EXTENSIONS_API_DIAL_DIAL_REGISTRY_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/threading/thread_checker.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/extensions/api/dial/dial_service.h"
#include "chrome/browser/profiles/profile.h"
#include "net/base/network_change_notifier.h"

namespace extensions {

class DialRegistry : public DialService::Observer,
                     public net::NetworkChangeNotifier::NetworkChangeObserver {
 public:
  typedef std::vector<DialDeviceData> DeviceList;

  enum DialErrorCode {
    DIAL_NO_LISTENERS = 0,
    DIAL_NO_INTERFACES,
    DIAL_NETWORK_DISCONNECTED,
    DIAL_CELLULAR_NETWORK,
    DIAL_SOCKET_ERROR,
    DIAL_UNKNOWN
  };

  class Observer {
   public:
    
    
    virtual void OnDialDeviceEvent(const DeviceList& devices) = 0;
    virtual void OnDialError(DialErrorCode type) = 0;

   protected:
    virtual ~Observer() {}
  };

  
  
  DialRegistry(Observer* dial_api,
               const base::TimeDelta& refresh_interval,
               const base::TimeDelta& expiration,
               const size_t max_devices);

  virtual ~DialRegistry();

  
  
  
  void OnListenerAdded();
  void OnListenerRemoved();

  
  
  bool DiscoverNow();

 protected:
  
  virtual DialService* CreateDialService();
  virtual void ClearDialService();

  
  virtual base::Time Now() const;

 protected:
  
  scoped_ptr<DialService> dial_;

 private:
  typedef base::hash_map<std::string, linked_ptr<DialDeviceData> >
      DeviceByIdMap;
  typedef std::map<std::string, linked_ptr<DialDeviceData> > DeviceByLabelMap;

  
  virtual void OnDiscoveryRequest(DialService* service) OVERRIDE;
  virtual void OnDeviceDiscovered(DialService* service,
                                  const DialDeviceData& device) OVERRIDE;
  virtual void OnDiscoveryFinished(DialService* service) OVERRIDE;
  virtual void OnError(DialService* service,
                       const DialService::DialServiceErrorCode& code) OVERRIDE;

  
  virtual void OnNetworkChanged(
      net::NetworkChangeNotifier::ConnectionType type) OVERRIDE;

  
  
  void StartPeriodicDiscovery();
  void StopPeriodicDiscovery();

  
  
  bool ReadyToDiscover();

  
  
  void Clear();

  
  void DoDiscovery();

  
  
  bool MaybeAddDevice(const linked_ptr<DialDeviceData>& device_data);

  
  
  bool PruneExpiredDevices();

  
  
  bool IsDeviceExpired(const DialDeviceData& device) const;

  
  void MaybeSendEvent();

  
  std::string NextLabel();

  
  int num_listeners_;

  
  int discovery_generation_;

  
  int registry_generation_;

  
  
  int last_event_discovery_generation_;

  
  
  int last_event_registry_generation_;

  
  int label_count_;

  
  base::TimeDelta refresh_interval_delta_;
  base::TimeDelta expiration_delta_;
  size_t max_devices_;

  
  DeviceByIdMap device_by_id_map_;

  
  
  DeviceByLabelMap device_by_label_map_;

  
  base::RepeatingTimer<DialRegistry> repeating_timer_;

  
  
  Observer* const dial_api_;

  
  base::ThreadChecker thread_checker_;

  FRIEND_TEST_ALL_PREFIXES(DialRegistryTest, TestAddRemoveListeners);
  FRIEND_TEST_ALL_PREFIXES(DialRegistryTest, TestNoDevicesDiscovered);
  FRIEND_TEST_ALL_PREFIXES(DialRegistryTest, TestDevicesDiscovered);
  FRIEND_TEST_ALL_PREFIXES(DialRegistryTest, TestDeviceExpires);
  FRIEND_TEST_ALL_PREFIXES(DialRegistryTest, TestExpiredDeviceIsRediscovered);
  FRIEND_TEST_ALL_PREFIXES(DialRegistryTest,
                           TestRemovingListenerDoesNotClearList);
  FRIEND_TEST_ALL_PREFIXES(DialRegistryTest, TestNetworkEventConnectionLost);
  FRIEND_TEST_ALL_PREFIXES(DialRegistryTest,
                           TestNetworkEventConnectionRestored);
  DISALLOW_COPY_AND_ASSIGN(DialRegistry);
};

}  

#endif  
