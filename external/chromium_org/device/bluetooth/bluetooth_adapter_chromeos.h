// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_ADAPTER_CHROMEOS_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_ADAPTER_CHROMEOS_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "chromeos/dbus/bluetooth_adapter_client.h"
#include "chromeos/dbus/bluetooth_device_client.h"
#include "chromeos/dbus/bluetooth_input_client.h"
#include "dbus/object_path.h"
#include "device/bluetooth/bluetooth_adapter.h"

namespace device {

class BluetoothAdapterFactory;

}  

namespace chromeos {

class BluetoothChromeOSTest;
class BluetoothDeviceChromeOS;

class BluetoothAdapterChromeOS
    : public device::BluetoothAdapter,
      private chromeos::BluetoothAdapterClient::Observer,
      private chromeos::BluetoothDeviceClient::Observer,
      private chromeos::BluetoothInputClient::Observer {
 public:
  
  virtual void AddObserver(
      device::BluetoothAdapter::Observer* observer) OVERRIDE;
  virtual void RemoveObserver(
      device::BluetoothAdapter::Observer* observer) OVERRIDE;
  virtual std::string GetAddress() const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual bool IsInitialized() const OVERRIDE;
  virtual bool IsPresent() const OVERRIDE;
  virtual bool IsPowered() const OVERRIDE;
  virtual void SetPowered(
      bool powered,
      const base::Closure& callback,
      const ErrorCallback& error_callback) OVERRIDE;
  virtual bool IsDiscovering() const OVERRIDE;
  virtual void StartDiscovering(
      const base::Closure& callback,
      const ErrorCallback& error_callback) OVERRIDE;
  virtual void StopDiscovering(
      const base::Closure& callback,
      const ErrorCallback& error_callback) OVERRIDE;
  virtual void ReadLocalOutOfBandPairingData(
      const device::BluetoothAdapter::BluetoothOutOfBandPairingDataCallback&
          callback,
      const ErrorCallback& error_callback) OVERRIDE;

 private:
  friend class device::BluetoothAdapterFactory;
  friend class BluetoothChromeOSTest;
  friend class BluetoothDeviceChromeOS;
  friend class BluetoothProfileChromeOS;
  friend class BluetoothProfileChromeOSTest;

  BluetoothAdapterChromeOS();
  virtual ~BluetoothAdapterChromeOS();

  
  virtual void AdapterAdded(const dbus::ObjectPath& object_path) OVERRIDE;
  virtual void AdapterRemoved(const dbus::ObjectPath& object_path) OVERRIDE;
  virtual void AdapterPropertyChanged(
      const dbus::ObjectPath& object_path,
      const std::string& property_name) OVERRIDE;

  
  virtual void DeviceAdded(const dbus::ObjectPath& object_path) OVERRIDE;
  virtual void DeviceRemoved(const dbus::ObjectPath& object_path) OVERRIDE;
  virtual void DevicePropertyChanged(const dbus::ObjectPath& object_path,
                                     const std::string& property_name) OVERRIDE;

  
  virtual void InputPropertyChanged(const dbus::ObjectPath& object_path,
                                    const std::string& property_name) OVERRIDE;

  
  
  BluetoothDeviceChromeOS* GetDeviceWithPath(
      const dbus::ObjectPath& object_path);

  
  
  void SetAdapter(const dbus::ObjectPath& object_path);

  
  
  void SetAdapterName();
  void OnSetAlias(bool success);

  
  
  void RemoveAdapter();

  
  void PoweredChanged(bool powered);
  void DiscoveringChanged(bool discovering);
  void PresentChanged(bool present);

  
  
  void NotifyDeviceChanged(BluetoothDeviceChromeOS* device);

  
  void OnSetPowered(const base::Closure& callback,
                    const ErrorCallback& error_callback,
                    bool success);

  
  void OnStartDiscovery(const base::Closure& callback);
  void OnStartDiscoveryError(const ErrorCallback& error_callback,
                             const std::string& error_name,
                             const std::string& error_message);

  
  void OnStopDiscovery(const base::Closure& callback);
  void OnStopDiscoveryError(const ErrorCallback& error_callback,
                            const std::string& error_name,
                            const std::string& error_message);

  
  dbus::ObjectPath object_path_;

  
  ObserverList<device::BluetoothAdapter::Observer> observers_;

  
  
  base::WeakPtrFactory<BluetoothAdapterChromeOS> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(BluetoothAdapterChromeOS);
};

}  

#endif  
