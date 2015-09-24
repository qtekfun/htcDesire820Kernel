// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_XINPUT_HIERARCHY_CHANGED_EVENT_LISTENER_H_
#define CHROME_BROWSER_CHROMEOS_XINPUT_HIERARCHY_CHANGED_EVENT_LISTENER_H_

#include "base/memory/singleton.h"
#include "base/message_loop/message_loop.h"
#include "base/observer_list.h"
#include "chrome/browser/chromeos/device_hierarchy_observer.h"

typedef union _XEvent XEvent;

namespace chromeos {

class XInputHierarchyChangedEventListener
    : public base::MessageLoopForUI::Observer {
 public:
  static XInputHierarchyChangedEventListener* GetInstance();

  void Stop();

  void AddObserver(DeviceHierarchyObserver* observer);
  void RemoveObserver(DeviceHierarchyObserver* observer);

 private:
  
  
  friend struct DefaultSingletonTraits<XInputHierarchyChangedEventListener>;

  XInputHierarchyChangedEventListener();
  virtual ~XInputHierarchyChangedEventListener();

  void Init();
  void StopImpl();

  
  virtual base::EventStatus WillProcessEvent(
      const base::NativeEvent& event) OVERRIDE;
  virtual void DidProcessEvent(const base::NativeEvent& event) OVERRIDE;

  
  virtual bool ProcessedXEvent(XEvent* xevent);

  
  void NotifyDeviceHierarchyChanged();

  bool stopped_;
  int xiopcode_;

  ObserverList<DeviceHierarchyObserver> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(XInputHierarchyChangedEventListener);
};

}  

#endif  
