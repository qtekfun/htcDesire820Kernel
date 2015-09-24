// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_NETWORK_STATE_HANDLER_OBSERVER_H_
#define CHROMEOS_NETWORK_NETWORK_STATE_HANDLER_OBSERVER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "chromeos/chromeos_export.h"

namespace chromeos {

class NetworkState;

class CHROMEOS_EXPORT NetworkStateHandlerObserver {
 public:
  NetworkStateHandlerObserver();
  virtual ~NetworkStateHandlerObserver();

  
  virtual void NetworkListChanged();

  
  virtual void DeviceListChanged();

  
  
  
  
  
  virtual void DefaultNetworkChanged(const NetworkState* network);

  
  virtual void NetworkConnectionStateChanged(const NetworkState* network);

  
  
  
  
  virtual void NetworkPropertiesUpdated(const NetworkState* network);

 private:
  DISALLOW_COPY_AND_ASSIGN(NetworkStateHandlerObserver);
};

}  

#endif  
