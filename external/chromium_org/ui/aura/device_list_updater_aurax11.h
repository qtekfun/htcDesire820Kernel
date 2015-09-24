// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_DEVICE_LIST_UPDATER_AURAX11_H_
#define UI_AURA_DEVICE_LIST_UPDATER_AURAX11_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/message_loop/message_pump_observer.h"

namespace aura {

class DeviceListUpdaterAuraX11 : public base::MessagePumpObserver {
 public:
  DeviceListUpdaterAuraX11();
  virtual ~DeviceListUpdaterAuraX11();

  
  virtual base::EventStatus WillProcessEvent(
      const base::NativeEvent& event) OVERRIDE;
  virtual void DidProcessEvent(
      const base::NativeEvent& event) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(DeviceListUpdaterAuraX11);
};

}  

#endif  
