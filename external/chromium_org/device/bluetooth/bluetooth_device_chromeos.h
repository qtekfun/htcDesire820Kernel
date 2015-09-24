// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_DEVICE_CHROMEOS_H
#define DEVICE_BLUETOOTH_BLUETOOTH_DEVICE_CHROMEOS_H

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/dbus/bluetooth_agent_service_provider.h"
#include "chromeos/dbus/bluetooth_device_client.h"
#include "dbus/object_path.h"
#include "device/bluetooth/bluetooth_device.h"

namespace chromeos {

class BluetoothAdapterChromeOS;

class BluetoothDeviceChromeOS
    : public device::BluetoothDevice,
      private chromeos::BluetoothAgentServiceProvider::Delegate {
 public:
  
  virtual uint32 GetBluetoothClass() const OVERRIDE;
  virtual std::string GetAddress() const OVERRIDE;
  virtual uint16 GetVendorID() const OVERRIDE;
  virtual uint16 GetProductID() const OVERRIDE;
  virtual uint16 GetDeviceID() const OVERRIDE;
  virtual bool IsPaired() const OVERRIDE;
  virtual bool IsConnected() const OVERRIDE;
  virtual bool IsConnectable() const OVERRIDE;
  virtual bool IsConnecting() const OVERRIDE;
  virtual ServiceList GetServices() const OVERRIDE;
  virtual void GetServiceRecords(
      const ServiceRecordsCallback& callback,
      const ErrorCallback& error_callback) OVERRIDE;
  virtual void ProvidesServiceWithName(
      const std::string& name,
      const ProvidesServiceCallback& callback) OVERRIDE;
  virtual bool ExpectingPinCode() const OVERRIDE;
  virtual bool ExpectingPasskey() const OVERRIDE;
  virtual bool ExpectingConfirmation() const OVERRIDE;
  virtual void Connect(
      device::BluetoothDevice::PairingDelegate* pairing_delegate,
      const base::Closure& callback,
      const ConnectErrorCallback& error_callback) OVERRIDE;
  virtual void SetPinCode(const std::string& pincode) OVERRIDE;
  virtual void SetPasskey(uint32 passkey) OVERRIDE;
  virtual void ConfirmPairing() OVERRIDE;
  virtual void RejectPairing() OVERRIDE;
  virtual void CancelPairing() OVERRIDE;
  virtual void Disconnect(
      const base::Closure& callback,
      const ErrorCallback& error_callback) OVERRIDE;
  virtual void Forget(const ErrorCallback& error_callback) OVERRIDE;
  virtual void ConnectToService(
      const std::string& service_uuid,
      const SocketCallback& callback) OVERRIDE;
  virtual void ConnectToProfile(
      device::BluetoothProfile* profile,
      const base::Closure& callback,
      const ErrorCallback& error_callback) OVERRIDE;
  virtual void SetOutOfBandPairingData(
      const device::BluetoothOutOfBandPairingData& data,
      const base::Closure& callback,
      const ErrorCallback& error_callback) OVERRIDE;
  virtual void ClearOutOfBandPairingData(
      const base::Closure& callback,
      const ErrorCallback& error_callback) OVERRIDE;

 protected:
   
  virtual std::string GetDeviceName() const OVERRIDE;

 private:
  friend class BluetoothAdapterChromeOS;

  BluetoothDeviceChromeOS(BluetoothAdapterChromeOS* adapter,
                          const dbus::ObjectPath& object_path);
  virtual ~BluetoothDeviceChromeOS();

  
  virtual void Release() OVERRIDE;
  virtual void RequestPinCode(const dbus::ObjectPath& device_path,
                              const PinCodeCallback& callback) OVERRIDE;
  virtual void DisplayPinCode(const dbus::ObjectPath& device_path,
                              const std::string& pincode) OVERRIDE;
  virtual void RequestPasskey(const dbus::ObjectPath& device_path,
                              const PasskeyCallback& callback) OVERRIDE;
  virtual void DisplayPasskey(const dbus::ObjectPath& device_path,
                              uint32 passkey, uint16 entered) OVERRIDE;
  virtual void RequestConfirmation(const dbus::ObjectPath& device_path,
                                   uint32 passkey,
                                   const ConfirmationCallback& callback)
      OVERRIDE;
  virtual void RequestAuthorization(const dbus::ObjectPath& device_path,
                                    const ConfirmationCallback& callback)
      OVERRIDE;
  virtual void AuthorizeService(const dbus::ObjectPath& device_path,
                                const std::string& uuid,
                                const ConfirmationCallback& callback) OVERRIDE;
  virtual void Cancel() OVERRIDE;

  
  
  void ConnectInternal(bool after_pairing,
                       const base::Closure& callback,
                       const ConnectErrorCallback& error_callback);
  void OnConnect(bool after_pairing,
                 const base::Closure& callback);
  void OnConnectError(bool after_pairing,
                      const ConnectErrorCallback& error_callback,
                      const std::string& error_name,
                      const std::string& error_message);

  
  
  void OnRegisterAgent(const base::Closure& callback,
                       const ConnectErrorCallback& error_callback);
  void OnRegisterAgentError(const ConnectErrorCallback& error_callback,
                            const std::string& error_name,
                            const std::string& error_message);

  
  void OnPair(const base::Closure& callback,
              const ConnectErrorCallback& error_callback);
  void OnPairError(const ConnectErrorCallback& error_callback,
                   const std::string& error_name,
                   const std::string& error_message);

  
  
  
  void OnCancelPairingError(const std::string& error_name,
                            const std::string& error_message);

  
  
  
  
  void SetTrusted();
  void OnSetTrusted(bool success);

  
  
  
  void UnregisterAgent();
  void OnUnregisterAgentError(const std::string& error_name,
                              const std::string& error_message);

  
  
  void OnDisconnect(const base::Closure& callback);
  void OnDisconnectError(const ErrorCallback& error_callback,
                         const std::string& error_name,
                         const std::string& error_message);

  
  
  
  void OnForgetError(const ErrorCallback& error_callback,
                     const std::string& error_name,
                     const std::string& error_message);

  
  
  bool RunPairingCallbacks(Status status);

  
  
  void OnConnectProfile(device::BluetoothProfile* profile,
                        const base::Closure& callback);
  void OnConnectProfileError(device::BluetoothProfile* profile,
                             const ErrorCallback& error_callback,
                             const std::string& error_name,
                             const std::string& error_message);

  
  const dbus::ObjectPath& object_path() const { return object_path_; }

  
  BluetoothAdapterChromeOS* adapter_;

  
  dbus::ObjectPath object_path_;

  
  int num_connecting_calls_;

  
  
  
  PairingDelegate* pairing_delegate_;

  
  
  bool pairing_delegate_used_;

  
  
  scoped_ptr<BluetoothAgentServiceProvider> agent_;

  
  
  
  PinCodeCallback pincode_callback_;
  PasskeyCallback passkey_callback_;
  ConfirmationCallback confirmation_callback_;

  
  
  base::WeakPtrFactory<BluetoothDeviceChromeOS> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(BluetoothDeviceChromeOS);
};

}  

#endif  
