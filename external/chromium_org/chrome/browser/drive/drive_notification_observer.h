// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DRIVE_DRIVE_NOTIFICATION_OBSERVER_H_
#define CHROME_BROWSER_DRIVE_DRIVE_NOTIFICATION_OBSERVER_H_

namespace drive {

class DriveNotificationObserver {
 public:
  
  virtual void OnNotificationReceived() = 0;

  
  virtual void OnPushNotificationEnabled(bool enabled) {}

 protected:
  virtual ~DriveNotificationObserver() {}
};

}  

#endif  
