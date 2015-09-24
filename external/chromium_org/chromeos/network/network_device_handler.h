// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_NETWORK_DEVICE_HANDLER_H_
#define CHROMEOS_NETWORK_NETWORK_DEVICE_HANDLER_H_

#include <string>

#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/network/network_handler.h"
#include "chromeos/network/network_handler_callbacks.h"

namespace base {

class Value;

}  

namespace chromeos {


class CHROMEOS_EXPORT NetworkDeviceHandler {
 public:

  
  static const char kErrorFailure[];
  static const char kErrorIncorrectPin[];
  static const char kErrorNotFound[];
  static const char kErrorNotSupported[];
  static const char kErrorPinBlocked[];
  static const char kErrorPinRequired[];
  static const char kErrorUnknown[];

  virtual ~NetworkDeviceHandler();

  
  
  void GetDeviceProperties(
      const std::string& device_path,
      const network_handler::DictionaryResultCallback& callback,
      const network_handler::ErrorCallback& error_callback) const;

  
  
  void SetDeviceProperty(
      const std::string& device_path,
      const std::string& name,
      const base::Value& value,
      const base::Closure& callback,
      const network_handler::ErrorCallback& error_callback);

  
  
  
  void RequestRefreshIPConfigs(
      const std::string& device_path,
      const base::Closure& callback,
      const network_handler::ErrorCallback& error_callback);

  
  
  
  
  
  
  
  
  
  
  void ProposeScan(
      const std::string& device_path,
      const base::Closure& callback,
      const network_handler::ErrorCallback& error_callback);

  
  
  
  
  
  
  void RegisterCellularNetwork(
      const std::string& device_path,
      const std::string& network_id,
      const base::Closure& callback,
      const network_handler::ErrorCallback& error_callback);

  
  
  
  
  
  
  
  
  
  void SetCarrier(
      const std::string& device_path,
      const std::string& carrier,
      const base::Closure& callback,
      const network_handler::ErrorCallback& error_callback);

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void RequirePin(
      const std::string& device_path,
      bool require_pin,
      const std::string& pin,
      const base::Closure& callback,
      const network_handler::ErrorCallback& error_callback);

  
  
  
  
  
  
  
  
  
  
  void EnterPin(
      const std::string& device_path,
      const std::string& pin,
      const base::Closure& callback,
      const network_handler::ErrorCallback& error_callback);

  
  
  
  
  
  
  
  
  
  
  void UnblockPin(
      const std::string& device_path,
      const std::string& puk,
      const std::string& new_pin,
      const base::Closure& callback,
      const network_handler::ErrorCallback& error_callback);

  
  
  
  
  
  
  
  
  
  
  
  void ChangePin(
      const std::string& device_path,
      const std::string& old_pin,
      const std::string& new_pin,
      const base::Closure& callback,
      const network_handler::ErrorCallback& error_callback);

 private:
  friend class NetworkHandler;
  friend class NetworkDeviceHandlerTest;

  NetworkDeviceHandler();

  DISALLOW_COPY_AND_ASSIGN(NetworkDeviceHandler);
};

}  

#endif  
