// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_BLUETOOTH_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_BLUETOOTH_OPTIONS_HANDLER_H_

#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "device/bluetooth/bluetooth_adapter.h"
#include "device/bluetooth/bluetooth_device.h"

namespace base {
class DictionaryValue;
}

namespace chromeos {
namespace options {

class BluetoothOptionsHandler
    : public ::options::OptionsPageUIHandler,
      public device::BluetoothAdapter::Observer,
      public device::BluetoothDevice::PairingDelegate {
 public:
  BluetoothOptionsHandler();
  virtual ~BluetoothOptionsHandler();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;
  virtual void InitializeHandler() OVERRIDE;
  virtual void InitializePage() OVERRIDE;

  void InitializeAdapter(scoped_refptr<device::BluetoothAdapter> adapter);

  
  
  
  void SendDeviceNotification(const device::BluetoothDevice* device,
                              base::DictionaryValue* params);

  
  
  
  
  
  
  
  
  virtual void RequestPinCode(device::BluetoothDevice* device) OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  virtual void RequestPasskey(device::BluetoothDevice* device) OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  virtual void DisplayPinCode(device::BluetoothDevice* device,
                              const std::string& pincode) OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  virtual void DisplayPasskey(
      device::BluetoothDevice* device, uint32 passkey) OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void KeysEntered(device::BluetoothDevice* device,
                           uint32 entered) OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  virtual void ConfirmPasskey(
      device::BluetoothDevice* device, uint32 passkey) OVERRIDE;

  
  
  
  
  
  virtual void DismissDisplayOrConfirm() OVERRIDE;

  
  
  
  
  void ReportError(const std::string& error, const std::string& address);

  
  virtual void AdapterPresentChanged(device::BluetoothAdapter* adapter,
                                     bool present) OVERRIDE;
  virtual void AdapterPoweredChanged(device::BluetoothAdapter* adapter,
                                     bool powered) OVERRIDE;
  virtual void DeviceAdded(device::BluetoothAdapter* adapter,
                           device::BluetoothDevice* device) OVERRIDE;
  virtual void DeviceChanged(device::BluetoothAdapter* adapter,
                             device::BluetoothDevice* device) OVERRIDE;
  virtual void DeviceRemoved(device::BluetoothAdapter* adapter,
                             device::BluetoothDevice* device) OVERRIDE;

 private:
  
  void DeviceConnecting(device::BluetoothDevice* device);

  
  
  void EnableChangeError();

  
  
  void FindDevicesError();

  
  
  void StopDiscoveryError();

  
  
  void Connected();

  
  
  
  void ConnectError(const std::string& address,
                    device::BluetoothDevice::ConnectErrorCode error_code);

  
  
  void DisconnectError(const std::string& address);

  
  
  void ForgetError(const std::string& address);

  
  
  
  void EnableChangeCallback(const base::ListValue* args);

  
  
  
  void FindDevicesCallback(const base::ListValue* args);

  
  
  
  
  
  void UpdateDeviceCallback(const base::ListValue* args);

  
  
  
  void StopDiscoveryCallback(const base::ListValue* args);

  
  
  
  void GetPairedDevicesCallback(const base::ListValue* args);

  
  scoped_refptr<device::BluetoothAdapter> adapter_;

  
  bool discovering_;

  
  std::string pairing_device_address_;
  std::string pairing_device_pairing_;
  std::string pairing_device_pincode_;
  int pairing_device_passkey_;
  int pairing_device_entered_;

  
  
  base::WeakPtrFactory<BluetoothOptionsHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(BluetoothOptionsHandler);
};

}  
}  

#endif  
