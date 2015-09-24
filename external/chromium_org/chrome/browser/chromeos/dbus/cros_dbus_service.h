// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DBUS_CROS_DBUS_SERVICE_H_
#define CHROME_BROWSER_CHROMEOS_DBUS_CROS_DBUS_SERVICE_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/threading/platform_thread.h"

namespace dbus {
class Bus;
class ExportedObject;
}

namespace chromeos {


class CrosDBusService {
 public:
  
  
  class ServiceProviderInterface {
   public:
    
    
    virtual void Start(
        scoped_refptr<dbus::ExportedObject> exported_object) = 0;

    virtual ~ServiceProviderInterface();
  };

  
  static void Initialize();
  
  static void Shutdown();

 protected:
  virtual ~CrosDBusService();

 private:
  
  
  friend class CrosDBusServiceTest;
  static void InitializeForTesting(
      dbus::Bus* bus,
      ServiceProviderInterface* proxy_resolution_service);
};

}  

#endif  
