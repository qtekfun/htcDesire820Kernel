// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_DISPATCHER_H
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_DISPATCHER_H

#include "base/base_export.h"
#include "base/event_types.h"

namespace base {

class BASE_EXPORT MessagePumpDispatcher {
 public:
  virtual ~MessagePumpDispatcher() {}

  
  
  virtual bool Dispatch(const NativeEvent& event) = 0;
};

}  

#endif  
