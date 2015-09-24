// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_DEVICE_WIN_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_DEVICE_WIN_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "device/bluetooth/bluetooth_device.h"
#include "device/bluetooth/bluetooth_task_manager_win.h"

namespace device {

class BluetoothAdapterWin;
class BluetoothServiceRecord;

class BluetoothDeviceWin : public BluetoothDevice {
 public:
  explicit BluetoothDeviceWin(
      const BluetoothTaskManagerWin::DeviceState& state);
  virtual ~BluetoothDeviceWin();

  
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
      PairingDelegate* pairing_delegate,
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
      const BluetoothOutOfBandPairingData& data,
      const base::Closure& callback,
      const ErrorCallback& error_callback) OVERRIDE;
  virtual void ClearOutOfBandPairingData(
      const base::Closure& callback,
      const ErrorCallback& error_callback) OVERRIDE;

  const BluetoothServiceRecord* GetServiceRecord(const std::string& uuid) const;

 protected:
  
  virtual std::string GetDeviceName() const OVERRIDE;

 private:
  friend class BluetoothAdapterWin;

  
  
  void SetVisible(bool visible);

  
  
  uint32 bluetooth_class_;

  
  std::string name_;

  
  std::string address_;

  
  
  bool paired_;
  bool connected_;

  
  
  bool visible_;

  
  ServiceList service_uuids_;
  ServiceRecordList service_record_list_;

  DISALLOW_COPY_AND_ASSIGN(BluetoothDeviceWin);
};

}  

#endif  
