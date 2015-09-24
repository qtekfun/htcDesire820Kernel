// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_BLUETOOTH_BLUETOOTH_EVENT_ROUTER_H_
#define CHROME_BROWSER_EXTENSIONS_API_BLUETOOTH_BLUETOOTH_EVENT_ROUTER_H_

#include <map>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "chrome/common/extensions/api/bluetooth.h"
#include "device/bluetooth/bluetooth_adapter.h"
#include "device/bluetooth/bluetooth_adapter_factory.h"
#include "device/bluetooth/bluetooth_socket.h"

class Profile;

namespace device {

class BluetoothDevice;
class BluetoothProfile;

}  

namespace extensions {

class ExtensionBluetoothEventRouter
    : public device::BluetoothAdapter::Observer {
 public:
  explicit ExtensionBluetoothEventRouter(Profile* profile);
  virtual ~ExtensionBluetoothEventRouter();

  
  
  bool IsBluetoothSupported() const;

  void GetAdapter(
      const device::BluetoothAdapterFactory::AdapterCallback& callback);

  
  void OnListenerAdded();

  
  void OnListenerRemoved();

  
  
  
  int RegisterSocket(scoped_refptr<device::BluetoothSocket> socket);

  
  
  bool ReleaseSocket(int id);

  
  
  
  void AddProfile(const std::string& uuid,
                  device::BluetoothProfile* bluetooth_profile);

  
  
  void RemoveProfile(const std::string& uuid);

  
  
  bool HasProfile(const std::string& uuid) const;

  
  
  device::BluetoothProfile* GetProfile(const std::string& uuid) const;

  
  scoped_refptr<device::BluetoothSocket> GetSocket(int id);

  
  
  void SetResponsibleForDiscovery(bool responsible);
  bool IsResponsibleForDiscovery() const;

  
  void SetSendDiscoveryEvents(bool should_send);

  
  void DispatchDeviceEvent(
      const std::string& event_name,
      const extensions::api::bluetooth::Device& device);

  
  
  void DispatchConnectionEvent(const std::string& extension_id,
                               const std::string& uuid,
                               const device::BluetoothDevice* device,
                               scoped_refptr<device::BluetoothSocket> socket);

  
  virtual void AdapterPresentChanged(device::BluetoothAdapter* adapter,
                                     bool present) OVERRIDE;
  virtual void AdapterPoweredChanged(device::BluetoothAdapter* adapter,
                                     bool has_power) OVERRIDE;
  virtual void AdapterDiscoveringChanged(device::BluetoothAdapter* adapter,
                                         bool discovering) OVERRIDE;
  virtual void DeviceAdded(device::BluetoothAdapter* adapter,
                           device::BluetoothDevice* device) OVERRIDE;

  
  void SetAdapterForTest(device::BluetoothAdapter* adapter) {
    adapter_ = adapter;
  }
 private:
  void InitializeAdapterIfNeeded();
  void InitializeAdapter(scoped_refptr<device::BluetoothAdapter> adapter);
  void MaybeReleaseAdapter();
  void DispatchAdapterStateEvent();

  bool send_discovery_events_;
  bool responsible_for_discovery_;

  Profile* profile_;
  scoped_refptr<device::BluetoothAdapter> adapter_;

  int num_event_listeners_;

  
  
  
  int next_socket_id_;

  typedef std::map<int, scoped_refptr<device::BluetoothSocket> > SocketMap;
  SocketMap socket_map_;

  typedef ScopedVector<extensions::api::bluetooth::Device>
      DeviceList;
  DeviceList discovered_devices_;

  
  typedef std::map<std::string, device::BluetoothProfile*> BluetoothProfileMap;
  BluetoothProfileMap bluetooth_profile_map_;

  base::WeakPtrFactory<ExtensionBluetoothEventRouter> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionBluetoothEventRouter);
};

}  

#endif  
