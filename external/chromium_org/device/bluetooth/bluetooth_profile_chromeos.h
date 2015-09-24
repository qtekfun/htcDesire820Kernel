// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_PROFILE_CHROMEOS_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_PROFILE_CHROMEOS_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/bluetooth_profile_service_provider.h"
#include "dbus/object_path.h"
#include "device/bluetooth/bluetooth_profile.h"

namespace dbus {

class FileDescriptor;

}  

namespace device {

class BluetoothAdapter;

}  

namespace chromeos {

class CHROMEOS_EXPORT BluetoothProfileChromeOS
    : public device::BluetoothProfile,
      private BluetoothProfileServiceProvider::Delegate {
 public:
  
  virtual void Unregister() OVERRIDE;
  virtual void SetConnectionCallback(
      const ConnectionCallback& callback) OVERRIDE;

  
  const std::string& uuid() const { return uuid_; }

 private:
  friend class BluetoothProfile;

  BluetoothProfileChromeOS();
  virtual ~BluetoothProfileChromeOS();

  
  
  
  void Init(const std::string& uuid,
            const device::BluetoothProfile::Options& options,
            const ProfileCallback& callback);

  
  virtual void Release() OVERRIDE;
  virtual void NewConnection(
      const dbus::ObjectPath& device_path,
      scoped_ptr<dbus::FileDescriptor> fd,
      const BluetoothProfileServiceProvider::Delegate::Options& options,
      const ConfirmationCallback& callback) OVERRIDE;
  virtual void RequestDisconnection(
      const dbus::ObjectPath& device_path,
      const ConfirmationCallback& callback) OVERRIDE;
  virtual void Cancel() OVERRIDE;

  
  
  void OnRegisterProfile(const ProfileCallback& callback);
  void OnRegisterProfileError(const ProfileCallback& callback,
                              const std::string& error_name,
                              const std::string& error_message);

  
  
  void OnUnregisterProfile();
  void OnUnregisterProfileError(const std::string& error_name,
                                const std::string& error_message);

  
  
  
  
  
  
  
  void GetAdapter(
      const dbus::ObjectPath& device_path,
      const BluetoothProfileServiceProvider::Delegate::Options& options,
      const ConfirmationCallback& callback,
      scoped_ptr<dbus::FileDescriptor> fd);
  void OnGetAdapter(
      const dbus::ObjectPath& device_path,
      const BluetoothProfileServiceProvider::Delegate::Options& options,
      const ConfirmationCallback& callback,
      scoped_ptr<dbus::FileDescriptor> fd,
      scoped_refptr<device::BluetoothAdapter>);

  
  std::string uuid_;

  
  dbus::ObjectPath object_path_;

  
  
  scoped_ptr<BluetoothProfileServiceProvider> profile_;

  
  
  ConnectionCallback connection_callback_;

  
  
  base::WeakPtrFactory<BluetoothProfileChromeOS> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(BluetoothProfileChromeOS);
};

}  

#endif  
