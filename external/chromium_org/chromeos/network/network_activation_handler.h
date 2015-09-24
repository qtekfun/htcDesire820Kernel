// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_NETWORK_ACTIVATION_HANDLER_H_
#define CHROMEOS_NETWORK_NETWORK_ACTIVATION_HANDLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/network/network_handler_callbacks.h"

namespace chromeos {

class CHROMEOS_EXPORT NetworkActivationHandler
    : public base::SupportsWeakPtr<NetworkActivationHandler> {
 public:
  
  
  
  static const char kErrorNotFound[];
  static const char kErrorShillError[];

  virtual ~NetworkActivationHandler();

  
  
  
  
  
  
  void Activate(const std::string& service_path,
                const std::string& carrier,
                const base::Closure& success_callback,
                const network_handler::ErrorCallback& error_callback);

  
  
  
  
  
  
  void CompleteActivation(const std::string& service_path,
                          const base::Closure& success_callback,
                          const network_handler::ErrorCallback& error_callback);

 private:
  friend class NetworkHandler;

  NetworkActivationHandler();

  
  void CallShillActivate(const std::string& service_path,
                         const std::string& carrier,
                         const base::Closure& success_callback,
                         const network_handler::ErrorCallback& error_callback);

  
  void CallShillCompleteActivation(
      const std::string& service_path,
      const base::Closure& success_callback,
      const network_handler::ErrorCallback& error_callback);

  
  
  void HandleShillSuccess(const std::string& service_path,
                          const base::Closure& success_callback);

  DISALLOW_COPY_AND_ASSIGN(NetworkActivationHandler);
};

}  

#endif  
