// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_NETWORK_NETWORK_ICON_ANIMATION_OBSERVER_H_
#define ASH_SYSTEM_CHROMEOS_NETWORK_NETWORK_ICON_ANIMATION_OBSERVER_H_

#include "ash/ash_export.h"

namespace ash {
namespace network_icon {

class ASH_EXPORT AnimationObserver {
 public:
  
  
  virtual void NetworkIconChanged() = 0;

 protected:
  virtual ~AnimationObserver() {}
};

}  
}  

#endif  
