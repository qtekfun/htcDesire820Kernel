// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_GEOLOCATION_HANDLER_H_
#define CHROMEOS_NETWORK_GEOLOCATION_HANDLER_H_

#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "chromeos/dbus/dbus_method_call_status.h"
#include "chromeos/dbus/shill_property_changed_observer.h"
#include "chromeos/network/network_handler.h"
#include "chromeos/network/network_util.h"

namespace base {
class DictionaryValue;
}

namespace chromeos {


class CHROMEOS_EXPORT GeolocationHandler : public ShillPropertyChangedObserver {
 public:
  virtual ~GeolocationHandler();

  
  
  
  bool GetWifiAccessPoints(WifiAccessPointVector* access_points, int64* age_ms);

  bool wifi_enabled() const { return wifi_enabled_; }

  
  virtual void OnPropertyChanged(const std::string& key,
                                 const base::Value& value) OVERRIDE;

 private:
  friend class NetworkHandler;
  friend class GeolocationHandlerTest;
  GeolocationHandler();

  void Init();

  
  void ManagerPropertiesCallback(DBusMethodCallStatus call_status,
                                 const base::DictionaryValue& properties);

  
  void HandlePropertyChanged(const std::string& key, const base::Value& value);

  
  void RequestWifiAccessPoints();

  
  void GeolocationCallback(DBusMethodCallStatus call_status,
                           const base::DictionaryValue& properties);

  
  bool wifi_enabled_;

  
  WifiAccessPointVector wifi_access_points_;
  base::Time geolocation_received_time_;

  
  base::WeakPtrFactory<GeolocationHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(GeolocationHandler);
};

}  

#endif  
