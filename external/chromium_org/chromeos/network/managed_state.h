// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_MANAGED_STATE_H_
#define CHROMEOS_NETWORK_MANAGED_STATE_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chromeos/chromeos_export.h"

namespace base {
class Value;
class DictionaryValue;
}

namespace chromeos {

class DeviceState;
class FavoriteState;
class NetworkState;
class NetworkTypePattern;

class CHROMEOS_EXPORT ManagedState {
 public:
  enum ManagedType {
    MANAGED_TYPE_NETWORK,
    MANAGED_TYPE_FAVORITE,
    MANAGED_TYPE_DEVICE
  };

  virtual ~ManagedState();

  
  
  static ManagedState* Create(ManagedType type, const std::string& path);

  
  
  NetworkState* AsNetworkState();
  DeviceState* AsDeviceState();
  FavoriteState* AsFavoriteState();

  
  
  
  
  
  
  
  
  virtual bool PropertyChanged(const std::string& key,
                               const base::Value& value) = 0;

  
  
  
  
  
  virtual bool InitialPropertiesReceived(
      const base::DictionaryValue& properties);

  const ManagedType managed_type() const { return managed_type_; }
  const std::string& path() const { return path_; }
  const std::string& name() const { return name_; }
  const std::string& type() const { return type_; }
  bool update_received() const { return update_received_; }
  void set_update_received() { update_received_ = true; }
  bool update_requested() const { return update_requested_; }
  void set_update_requested(bool update_requested) {
    update_requested_ = update_requested;
  }

  bool Matches(const NetworkTypePattern& pattern) const;

 protected:
  ManagedState(ManagedType type, const std::string& path);

  
  bool ManagedStatePropertyChanged(const std::string& key,
                                   const base::Value& value);

  
  
  bool GetBooleanValue(const std::string& key,
                       const base::Value& value,
                       bool* out_value);
  bool GetIntegerValue(const std::string& key,
                       const base::Value& value,
                       int* out_value);
  bool GetStringValue(const std::string& key,
                      const base::Value& value,
                      std::string* out_value);
  bool GetUInt32Value(const std::string& key,
                      const base::Value& value,
                      uint32* out_value);

  void set_name(const std::string& name) { name_ = name; }

 private:
  friend class NetworkChangeNotifierChromeosUpdateTest;

  ManagedType managed_type_;

  
  std::string path_;

  
  std::string name_;  
  std::string type_;  

  
  bool update_received_;

  
  bool update_requested_;

  DISALLOW_COPY_AND_ASSIGN(ManagedState);
};

}  

#endif  
