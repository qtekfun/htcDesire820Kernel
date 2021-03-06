// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_BLOCKING_METHOD_CALLER_H_
#define CHROMEOS_DBUS_BLOCKING_METHOD_CALLER_H_

#include "base/callback.h"
#include "base/synchronization/waitable_event.h"
#include "chromeos/chromeos_export.h"
#include "dbus/message.h"

namespace dbus {

class Bus;
class ObjectProxy;

}  

namespace chromeos {

class CHROMEOS_EXPORT BlockingMethodCaller {
 public:
  BlockingMethodCaller(dbus::Bus* bus, dbus::ObjectProxy* proxy);
  virtual ~BlockingMethodCaller();

  
  scoped_ptr<dbus::Response> CallMethodAndBlock(dbus::MethodCall* method_call);

 private:
  dbus::Bus* bus_;
  dbus::ObjectProxy* proxy_;
  base::WaitableEvent on_blocking_method_call_;

  DISALLOW_COPY_AND_ASSIGN(BlockingMethodCaller);
};

}  

#endif  
