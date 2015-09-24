// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_DEVICE_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_DEVICE_H_

#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/strings/string16.h"

namespace device {

class BluetoothProfile;
class BluetoothServiceRecord;
class BluetoothSocket;

struct BluetoothOutOfBandPairingData;

class BluetoothDevice {
 public:
  
  
  
  enum DeviceType {
    DEVICE_UNKNOWN,
    DEVICE_COMPUTER,
    DEVICE_PHONE,
    DEVICE_MODEM,
    DEVICE_AUDIO,
    DEVICE_CAR_AUDIO,
    DEVICE_VIDEO,
    DEVICE_PERIPHERAL,
    DEVICE_JOYSTICK,
    DEVICE_GAMEPAD,
    DEVICE_KEYBOARD,
    DEVICE_MOUSE,
    DEVICE_TABLET,
    DEVICE_KEYBOARD_MOUSE_COMBO
  };

  
  
  enum ConnectErrorCode {
    ERROR_UNKNOWN,
    ERROR_INPROGRESS,
    ERROR_FAILED,
    ERROR_AUTH_FAILED,
    ERROR_AUTH_CANCELED,
    ERROR_AUTH_REJECTED,
    ERROR_AUTH_TIMEOUT,
    ERROR_UNSUPPORTED_DEVICE
  };

  
  class Observer {
   public:
    virtual ~Observer() {}

    
  };

  
  class PairingDelegate {
   public:
    virtual ~PairingDelegate() {}

    
    
    
    
    
    
    
    
    virtual void RequestPinCode(BluetoothDevice* device) = 0;

    
    
    
    
    
    
    
    
    
    virtual void RequestPasskey(BluetoothDevice* device) = 0;

    
    
    
    
    
    
    
    
    
    virtual void DisplayPinCode(BluetoothDevice* device,
                                const std::string& pincode) = 0;

    
    
    
    
    
    
    
    
    
    virtual void DisplayPasskey(BluetoothDevice* device,
                                uint32 passkey) = 0;

    
    
    
    
    
    
    
    
    
    
    
    
    
    virtual void KeysEntered(BluetoothDevice* device,
                             uint32 entered) = 0;

    
    
    
    
    
    
    
    
    
    
    
    
    virtual void ConfirmPasskey(BluetoothDevice* device,
                                uint32 passkey) = 0;

    
    
    
    virtual void DismissDisplayOrConfirm() = 0;
  };

  
  
  static bool IsUUIDValid(const std::string& uuid);

  virtual ~BluetoothDevice();

  
  
  virtual uint32 GetBluetoothClass() const = 0;

  
  
  virtual std::string GetAddress() const = 0;

  
  virtual uint16 GetVendorID() const = 0;

  
  virtual uint16 GetProductID() const = 0;

  
  
  virtual uint16 GetDeviceID() const = 0;

  
  
  
  virtual string16 GetName() const;

  
  
  
  
  DeviceType GetDeviceType() const;

  
  
  bool IsPairable() const;

  
  virtual bool IsPaired() const = 0;

  
  
  
  
  
  
  virtual bool IsConnected() const = 0;

  
  
  virtual bool IsConnectable() const = 0;

  
  
  
  virtual bool IsConnecting() const = 0;

  
  
  typedef std::vector<std::string> ServiceList;
  virtual ServiceList GetServices() const = 0;

  
  
  typedef base::Callback<void()> ErrorCallback;

  
  
  
  typedef base::Callback<void(enum ConnectErrorCode)> ConnectErrorCallback;

  
  
  typedef ScopedVector<BluetoothServiceRecord> ServiceRecordList;
  typedef base::Callback<void(const ServiceRecordList&)> ServiceRecordsCallback;
  virtual void GetServiceRecords(const ServiceRecordsCallback& callback,
                                 const ErrorCallback& error_callback) = 0;

  
  virtual bool ProvidesServiceWithUUID(const std::string& uuid) const;

  
  
  typedef base::Callback<void(bool)> ProvidesServiceCallback;

  
  virtual void ProvidesServiceWithName(
      const std::string& name,
      const ProvidesServiceCallback& callback) = 0;

  
  
  virtual bool ExpectingPinCode() const = 0;

  
  
  virtual bool ExpectingPasskey() const = 0;

  
  
  virtual bool ExpectingConfirmation() const = 0;

  
  
  
  typedef base::Callback<void(scoped_refptr<BluetoothSocket>)>
      SocketCallback;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Connect(PairingDelegate* pairing_delegate,
                       const base::Closure& callback,
                       const ConnectErrorCallback& error_callback) = 0;

  
  
  
  
  virtual void SetPinCode(const std::string& pincode) = 0;

  
  
  
  
  
  virtual void SetPasskey(uint32 passkey) = 0;

  
  
  virtual void ConfirmPairing() = 0;

  
  virtual void RejectPairing() = 0;

  
  
  virtual void CancelPairing() = 0;

  
  
  
  
  
  virtual void Disconnect(const base::Closure& callback,
                          const ErrorCallback& error_callback) = 0;

  
  
  
  
  
  
  
  virtual void Forget(const ErrorCallback& error_callback) = 0;

  
  
  
  
  
  
  virtual void ConnectToService(const std::string& service_uuid,
                                const SocketCallback& callback) = 0;

  
  
  
  
  virtual void ConnectToProfile(BluetoothProfile* profile,
                                const base::Closure& callback,
                                const ErrorCallback& error_callback) = 0;

  
  
  virtual void SetOutOfBandPairingData(
      const BluetoothOutOfBandPairingData& data,
      const base::Closure& callback,
      const ErrorCallback& error_callback) = 0;

  
  
  virtual void ClearOutOfBandPairingData(
      const base::Closure& callback,
      const ErrorCallback& error_callback) = 0;

 protected:
  BluetoothDevice();

  
  virtual std::string GetDeviceName() const = 0;

 private:
  
  
  string16 GetAddressWithLocalizedDeviceTypeName() const;
};

}  

#endif  
