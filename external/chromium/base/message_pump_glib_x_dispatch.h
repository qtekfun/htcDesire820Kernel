// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_PUMP_GLIB_X_DISPATCH_H
#define BASE_MESSAGE_PUMP_GLIB_X_DISPATCH_H

#include "base/message_pump.h"
#include "base/message_pump_glib.h"

typedef union _XEvent XEvent;

namespace base {

class MessagePumpGlibXDispatcher : public MessagePumpForUI::Dispatcher {
 public:
  enum DispatchStatus {
    EVENT_IGNORED,    
    EVENT_PROCESSED,  
    EVENT_QUIT        
                      
  };

  
  
  
  virtual DispatchStatus DispatchX(XEvent* xevent) = 0;
};

}  

#endif  
