// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_NET_NETWORK_CHANGE_NOTIFIER_CHROMEOS_H_
#define CHROME_BROWSER_CHROMEOS_NET_NETWORK_CHANGE_NOTIFIER_CHROMEOS_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/chromeos/cros/network_library.h"
#include "net/base/network_change_notifier.h"

namespace chromeos {

class NetworkChangeNotifierChromeos
    : public net::NetworkChangeNotifier,
      public chromeos::NetworkLibrary::NetworkManagerObserver,
      public chromeos::NetworkLibrary::NetworkObserver {
 public:
  NetworkChangeNotifierChromeos();
  virtual ~NetworkChangeNotifierChromeos();

 private:
  
  virtual bool IsCurrentlyOffline() const OVERRIDE;

  
  virtual void OnNetworkManagerChanged(chromeos::NetworkLibrary* obj) OVERRIDE;

  
  virtual void OnNetworkChanged(chromeos::NetworkLibrary* cros,
                                const chromeos::Network* network) OVERRIDE;

  
  void UpdateNetworkState(chromeos::NetworkLibrary* cros);

  
  bool has_active_network_;
  
  chromeos::ConnectivityState connectivity_state_;
  
  std::string service_path_;
  
  std::string ip_address_;

  ScopedRunnableMethodFactory<NetworkChangeNotifierChromeos> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(NetworkChangeNotifierChromeos);
};

}  

#endif  
