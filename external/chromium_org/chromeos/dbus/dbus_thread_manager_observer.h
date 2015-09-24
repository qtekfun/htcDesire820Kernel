// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_DBUS_THREAD_MANAGER_OBSERVER_H_
#define CHROMEOS_DBUS_DBUS_THREAD_MANAGER_OBSERVER_H_

namespace chromeos {

class DBusThreadManager;

class DBusThreadManagerObserver {
 public:
  
  
  virtual void OnDBusThreadManagerDestroying(DBusThreadManager* manager) = 0;

 protected:
  virtual ~DBusThreadManagerObserver() {}
};

}  

#endif  
