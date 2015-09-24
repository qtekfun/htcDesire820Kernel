// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DEVICE_UMA_H_
#define CHROME_BROWSER_CHROMEOS_DEVICE_UMA_H_

#include "base/basictypes.h"
#include "base/event_types.h"
#include "base/message_loop/message_loop.h"

template <typename T> struct DefaultSingletonTraits;

namespace chromeos {

class DeviceUMA : public base::MessageLoopForUI::Observer {
 public:
  
  
  static DeviceUMA* GetInstance();

  void Stop();

 private:
  friend struct DefaultSingletonTraits<DeviceUMA>;

  DeviceUMA();
  virtual ~DeviceUMA();

  
  void AddMessageLoopObserver();
  void RemoveMessageLoopObserver();

  
  virtual base::EventStatus WillProcessEvent(
      const base::NativeEvent& event) OVERRIDE;
  virtual void DidProcessEvent(const base::NativeEvent& event) OVERRIDE;

  
  void CheckTouchpadEvent(const base::NativeEvent& event);

  
  void CheckIncomingEvent(const base::NativeEvent& event);

  bool is_observing_;

  DISALLOW_COPY_AND_ASSIGN(DeviceUMA);
};

}  

#endif  
