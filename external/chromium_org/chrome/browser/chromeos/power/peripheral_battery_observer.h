// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POWER_PERIPHERAL_BATTERY_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_POWER_PERIPHERAL_BATTERY_OBSERVER_H_

#include <map>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "base/test/simple_test_tick_clock.h"
#include "chromeos/dbus/power_manager_client.h"
#include "device/bluetooth/bluetooth_adapter.h"

namespace chromeos {

class BluetoothDevice;
class PeripheralBatteryObserverTest;

class PeripheralBatteryObserver : public PowerManagerClient::Observer,
                                  public device::BluetoothAdapter::Observer {
 public:
  
  PeripheralBatteryObserver();
  virtual ~PeripheralBatteryObserver();

  void set_testing_clock(base::SimpleTestTickClock* clock) {
    testing_clock_ = clock;
  }

  
  virtual void PeripheralBatteryStatusReceived(const std::string& path,
                                               const std::string& name,
                                               int level) OVERRIDE;

  
  virtual void DeviceChanged(device::BluetoothAdapter* adapter,
                             device::BluetoothDevice* device) OVERRIDE;
  virtual void DeviceRemoved(device::BluetoothAdapter* adapter,
                             device::BluetoothDevice* device) OVERRIDE;

 private:
  friend class PeripheralBatteryObserverTest;
  FRIEND_TEST_ALL_PREFIXES(PeripheralBatteryObserverTest, Basic);
  FRIEND_TEST_ALL_PREFIXES(PeripheralBatteryObserverTest, InvalidBatteryInfo);
  FRIEND_TEST_ALL_PREFIXES(PeripheralBatteryObserverTest, DeviceRemove);

  struct BatteryInfo {
    BatteryInfo() : level(-1) {}
    BatteryInfo(const std::string& name,
                int level,
                base::TimeTicks notification_timestamp)
        : name(name),
          level(level),
          last_notification_timestamp(notification_timestamp) {
    }

    
    std::string name;
    
    int level;
    base::TimeTicks last_notification_timestamp;
  };

  void InitializeOnBluetoothReady(
      scoped_refptr<device::BluetoothAdapter> adapter);

  void RemoveBattery(const std::string& address);

  
  
  bool PostNotification(const std::string& address, const BatteryInfo& battery);

  void CancelNotification(const std::string& address);

  
  
  std::map<std::string, BatteryInfo> batteries_;

  
  
  scoped_refptr<device::BluetoothAdapter> bluetooth_adapter_;

  
  base::SimpleTestTickClock* testing_clock_;

  scoped_ptr<base::WeakPtrFactory<PeripheralBatteryObserver> > weakptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PeripheralBatteryObserver);
};

}  

#endif  
