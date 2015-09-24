// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_DBUS_THREAD_MANAGER_H_
#define CHROMEOS_DBUS_DBUS_THREAD_MANAGER_H_

#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"

namespace base {
class Thread;
};

namespace dbus {
class Bus;
class ObjectPath;
};

namespace chromeos {

class DBusThreadManagerObserver;

class BluetoothAdapterClient;
class BluetoothAgentManagerClient;
class BluetoothDeviceClient;
class BluetoothInputClient;
class BluetoothProfileManagerClient;
class CrasAudioClient;
class CrosDisksClient;
class CryptohomeClient;
class DBusClient;
class DebugDaemonClient;
class GsmSMSClient;
class ImageBurnerClient;
class IntrospectableClient;
class ModemMessagingClient;
class NfcAdapterClient;
class NfcDeviceClient;
class NfcManagerClient;
class NfcRecordClient;
class NfcTagClient;
class PermissionBrokerClient;
class PowerManagerClient;
class PowerPolicyController;
class SessionManagerClient;
class ShillDeviceClient;
class ShillIPConfigClient;
class ShillManagerClient;
class ShillProfileClient;
class ShillServiceClient;
class SMSClient;
class SystemClockClient;
class UpdateEngineClient;

class CHROMEOS_EXPORT DBusThreadManager {
 public:
  
  
  
  static void Initialize();

  
  
  
  
  
  
  static void SetInstanceForTesting(DBusThreadManager* dbus_thread_manager);

  
  
  
  
  static void InitializeForTesting(DBusThreadManager* dbus_thread_manager);

  
  static void InitializeWithStub();

  
  
  static bool IsInitialized();

  
  static void Shutdown();

  
  static DBusThreadManager* Get();

  
  virtual void AddObserver(DBusThreadManagerObserver* observer) = 0;
  virtual void RemoveObserver(DBusThreadManagerObserver* observer) = 0;

  
  virtual dbus::Bus* GetSystemBus() = 0;

  
  
  virtual BluetoothAdapterClient* GetBluetoothAdapterClient() = 0;
  virtual BluetoothAgentManagerClient* GetBluetoothAgentManagerClient() = 0;
  virtual BluetoothDeviceClient* GetBluetoothDeviceClient() = 0;
  virtual BluetoothInputClient* GetBluetoothInputClient() = 0;
  virtual BluetoothProfileManagerClient* GetBluetoothProfileManagerClient() = 0;
  virtual CrasAudioClient* GetCrasAudioClient() = 0;
  virtual CrosDisksClient* GetCrosDisksClient() = 0;
  virtual CryptohomeClient* GetCryptohomeClient() = 0;
  virtual DebugDaemonClient* GetDebugDaemonClient() = 0;
  virtual GsmSMSClient* GetGsmSMSClient() = 0;
  virtual ImageBurnerClient* GetImageBurnerClient() = 0;
  virtual IntrospectableClient* GetIntrospectableClient() = 0;
  virtual ModemMessagingClient* GetModemMessagingClient() = 0;
  virtual NfcAdapterClient* GetNfcAdapterClient() = 0;
  virtual NfcDeviceClient* GetNfcDeviceClient() = 0;
  virtual NfcManagerClient* GetNfcManagerClient() = 0;
  virtual NfcRecordClient* GetNfcRecordClient() = 0;
  virtual NfcTagClient* GetNfcTagClient() = 0;
  virtual PermissionBrokerClient* GetPermissionBrokerClient() = 0;
  virtual PowerManagerClient* GetPowerManagerClient() = 0;
  virtual PowerPolicyController* GetPowerPolicyController() = 0;
  virtual SessionManagerClient* GetSessionManagerClient() = 0;
  virtual ShillDeviceClient* GetShillDeviceClient() = 0;
  virtual ShillIPConfigClient* GetShillIPConfigClient() = 0;
  virtual ShillManagerClient* GetShillManagerClient() = 0;
  virtual ShillServiceClient* GetShillServiceClient() = 0;
  virtual ShillProfileClient* GetShillProfileClient() = 0;
  virtual SMSClient* GetSMSClient() = 0;
  virtual SystemClockClient* GetSystemClockClient() = 0;
  virtual UpdateEngineClient* GetUpdateEngineClient() = 0;

  virtual ~DBusThreadManager();

 protected:
  DBusThreadManager();

 private:
  
  
  
  static void InitializeClients();

  
  static void InitClient(DBusClient* client);

  DISALLOW_COPY_AND_ASSIGN(DBusThreadManager);
};

}  

#endif  
