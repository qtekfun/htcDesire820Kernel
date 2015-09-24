// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_SYSTEM_CLOCK_CLIENT_H_
#define CHROMEOS_DBUS_SYSTEM_CLOCK_CLIENT_H_

#include "base/observer_list.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"

namespace chromeos {

class CHROMEOS_EXPORT SystemClockClient : public DBusClient {
 public:
  
  class Observer {
   public:
    
    virtual void SystemClockUpdated() {}
   protected:
    virtual ~Observer() {}
  };

  virtual ~SystemClockClient();

  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;
  
  virtual bool HasObserver(Observer* observer) = 0;

  
  static SystemClockClient* Create();

 protected:
  
  SystemClockClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(SystemClockClient);
};

}  

#endif  
