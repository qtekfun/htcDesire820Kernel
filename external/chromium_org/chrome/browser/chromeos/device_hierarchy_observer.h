// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DEVICE_HIERARCHY_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_DEVICE_HIERARCHY_OBSERVER_H_

namespace chromeos {

class DeviceHierarchyObserver {
 public:
  virtual void DeviceHierarchyChanged() = 0;

  
  
  virtual void DeviceAdded(int device_id) = 0;
  virtual void DeviceRemoved(int device_id) = 0;

  
  
  
  virtual void DeviceKeyPressedOrReleased(int device_id) = 0;

 protected:
  virtual ~DeviceHierarchyObserver() {}
};

}  

#endif  
