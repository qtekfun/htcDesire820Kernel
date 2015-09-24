// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_NFC_NFC_ADAPTER_CHROMEOS_H_
#define DEVICE_NFC_NFC_ADAPTER_CHROMEOS_H_

#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "chromeos/dbus/nfc_adapter_client.h"
#include "chromeos/dbus/nfc_device_client.h"
#include "chromeos/dbus/nfc_tag_client.h"
#include "dbus/object_path.h"
#include "device/nfc/nfc_adapter.h"

namespace device {

class NfcAdapterFactory;

}  

namespace chromeos {

class NfcAdapterChromeOS : public device::NfcAdapter,
                           public chromeos::NfcAdapterClient::Observer,
                           public chromeos::NfcDeviceClient::Observer,
                           public chromeos::NfcTagClient::Observer {
 public:
  
  virtual void AddObserver(NfcAdapter::Observer* observer) OVERRIDE;
  virtual void RemoveObserver(NfcAdapter::Observer* observer) OVERRIDE;
  virtual bool IsPresent() const OVERRIDE;
  virtual bool IsPowered() const OVERRIDE;
  virtual bool IsPolling() const OVERRIDE;
  virtual bool IsInitialized() const OVERRIDE;
  virtual void SetPowered(bool powered,
                          const base::Closure& callback,
                          const ErrorCallback& error_callback) OVERRIDE;
  virtual void StartPolling(const base::Closure& callback,
                            const ErrorCallback& error_callback) OVERRIDE;
  virtual void StopPolling(const base::Closure& callback,
                           const ErrorCallback& error_callback) OVERRIDE;

 private:
  friend class device::NfcAdapterFactory;
  friend class NfcChromeOSTest;

  NfcAdapterChromeOS();
  virtual ~NfcAdapterChromeOS();

  
  virtual void AdapterAdded(const dbus::ObjectPath& object_path) OVERRIDE;
  virtual void AdapterRemoved(const dbus::ObjectPath& object_path) OVERRIDE;
  virtual void AdapterPropertyChanged(
      const dbus::ObjectPath& object_path,
      const std::string& property_name) OVERRIDE;

  
  virtual void DeviceAdded(const dbus::ObjectPath& object_path) OVERRIDE;
  virtual void DeviceRemoved(const dbus::ObjectPath& object_path) OVERRIDE;
  virtual void DevicePropertyChanged(const dbus::ObjectPath& object_path,
                                     const std::string& property_name) OVERRIDE;

  
  virtual void TagAdded(const dbus::ObjectPath& object_path) OVERRIDE;
  virtual void TagRemoved(const dbus::ObjectPath& object_path) OVERRIDE;
  virtual void TagPropertyChanged(const dbus::ObjectPath& object_path,
                                  const std::string& property_name) OVERRIDE;

  
  
  void SetAdapter(const dbus::ObjectPath& object_path);

  
  
  void RemoveAdapter();

  
  void PoweredChanged(bool powered);
  void PollingChanged(bool polling);
  void PresentChanged(bool present);

  
  void OnSetPowered(const base::Closure& callback,
                    const ErrorCallback& error_callback,
                    bool success);

  
  void OnStartPolling(const base::Closure& callback);
  void OnStartPollingError(const ErrorCallback& error_callback,
                           const std::string& error_name,
                           const std::string& error_message);

  
  void OnStopPolling(const base::Closure& callback);
  void OnStopPollingError(const ErrorCallback& error_callback,
                          const std::string& error_name,
                          const std::string& error_message);

  
  dbus::ObjectPath object_path_;

  
  ObserverList<device::NfcAdapter::Observer> observers_;

  
  
  base::WeakPtrFactory<NfcAdapterChromeOS> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(NfcAdapterChromeOS);
};

}  

#endif  
