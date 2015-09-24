// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_DBUS_CLIENT_H_
#define CHROMEOS_DBUS_DBUS_CLIENT_H_

#include "base/basictypes.h"

namespace dbus {
class Bus;
};

namespace chromeos {

class DBusClient {
 protected:
  friend class DBusThreadManager;

  virtual ~DBusClient() {}

  
  
  
  virtual void Init(dbus::Bus* bus) = 0;

 private:
  DISALLOW_ASSIGN(DBusClient);
};

}  

#endif  
