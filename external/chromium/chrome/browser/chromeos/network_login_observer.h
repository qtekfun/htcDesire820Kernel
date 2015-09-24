// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_NETWORK_LOGIN_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_NETWORK_LOGIN_OBSERVER_H_
#pragma once

#include <map>
#include <string>

#include "base/basictypes.h"
#include "chrome/browser/chromeos/cros/network_library.h"

namespace views {
class WindowDelegate;
}

namespace chromeos {


class NetworkLoginObserver : public NetworkLibrary::NetworkManagerObserver {
 public:
  explicit NetworkLoginObserver(NetworkLibrary* netlib);
  virtual ~NetworkLoginObserver();

  typedef std::map<std::string, bool> NetworkFailureMap;
 private:
  virtual void CreateModalPopup(views::WindowDelegate* view);

  virtual void RefreshStoredNetworks(const WifiNetworkVector& wifi_networks,
                                     const VirtualNetworkVector& vpn_networks);

  
  virtual void OnNetworkManagerChanged(NetworkLibrary* obj);

  
  NetworkFailureMap network_failures_;

  DISALLOW_COPY_AND_ASSIGN(NetworkLoginObserver);
};

}  

#endif  
