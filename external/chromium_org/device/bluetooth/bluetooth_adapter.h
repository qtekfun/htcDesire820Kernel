// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_ADAPTER_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_ADAPTER_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"

namespace device {

class BluetoothDevice;

struct BluetoothOutOfBandPairingData;

class BluetoothAdapter : public base::RefCounted<BluetoothAdapter> {
 public:
  
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    
    virtual void AdapterPresentChanged(BluetoothAdapter* adapter,
                                       bool present) {}

    
    
    
    virtual void AdapterPoweredChanged(BluetoothAdapter* adapter,
                                       bool powered) {}

    
    
    
    virtual void AdapterDiscoveringChanged(BluetoothAdapter* adapter,
                                           bool discovering) {}

    
    
    
    virtual void DeviceAdded(BluetoothAdapter* adapter,
                             BluetoothDevice* device) {}

    
    
    
    virtual void DeviceChanged(BluetoothAdapter* adapter,
                               BluetoothDevice* device) {}

    
    
    
    virtual void DeviceRemoved(BluetoothAdapter* adapter,
                               BluetoothDevice* device) {}
  };

  
  
  typedef base::Callback<void()> ErrorCallback;

  
  
  typedef base::Callback<void(const BluetoothOutOfBandPairingData& data)>
      BluetoothOutOfBandPairingDataCallback;

  
  
  
  virtual void AddObserver(BluetoothAdapter::Observer* observer) = 0;
  virtual void RemoveObserver(
      BluetoothAdapter::Observer* observer) = 0;

  
  
  virtual std::string GetAddress() const = 0;

  
  virtual std::string GetName() const = 0;

  
  virtual bool IsInitialized() const = 0;

  
  
  
  virtual bool IsPresent() const = 0;

  
  virtual bool IsPowered() const = 0;

  
  
  
  virtual void SetPowered(bool powered,
                          const base::Closure& callback,
                          const ErrorCallback& error_callback) = 0;

  
  virtual bool IsDiscovering() const = 0;

  
  
  
  
  
  
  
  
  
  
  virtual void StartDiscovering(const base::Closure& callback,
                                const ErrorCallback& error_callback) = 0;

  
  
  
  
  
  virtual void StopDiscovering(const base::Closure& callback,
                               const ErrorCallback& error_callback) = 0;

  
  
  
  typedef std::vector<BluetoothDevice*> DeviceList;
  virtual DeviceList GetDevices();
  typedef std::vector<const BluetoothDevice*> ConstDeviceList;
  virtual ConstDeviceList GetDevices() const;

  
  
  virtual BluetoothDevice* GetDevice(const std::string& address);
  virtual const BluetoothDevice* GetDevice(
      const std::string& address) const;

  
  virtual void ReadLocalOutOfBandPairingData(
      const BluetoothOutOfBandPairingDataCallback& callback,
      const ErrorCallback& error_callback) = 0;

 protected:
  friend class base::RefCounted<BluetoothAdapter>;
  BluetoothAdapter();
  virtual ~BluetoothAdapter();

  
  
  
  
  typedef std::map<const std::string, BluetoothDevice*> DevicesMap;
  DevicesMap devices_;
};

}  

#endif  
