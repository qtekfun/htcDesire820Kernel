// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_NETWORK_NETWORK_DETAILED_VIEW_H
#define ASH_SYSTEM_CHROMEOS_NETWORK_NETWORK_DETAILED_VIEW_H

#include "ash/system/tray/tray_details_view.h"
#include "chromeos/network/network_state_handler.h"

namespace ash {
namespace internal {

namespace tray {

class NetworkDetailedView : public TrayDetailsView {
 public:
  enum DetailedViewType {
    LIST_VIEW,
    STATE_LIST_VIEW,
    WIFI_VIEW,
  };

  explicit NetworkDetailedView(SystemTrayItem* owner)
      : TrayDetailsView(owner) {
  }

  virtual void Init() = 0;

  virtual DetailedViewType GetViewType() const = 0;

  
  
  virtual void ManagerChanged() = 0;

  
  
  virtual void NetworkListChanged() = 0;

  
  
  virtual void NetworkServiceChanged(const chromeos::NetworkState* network) = 0;

 protected:
  virtual ~NetworkDetailedView() {}
};

}  

}  
}  

#endif  
