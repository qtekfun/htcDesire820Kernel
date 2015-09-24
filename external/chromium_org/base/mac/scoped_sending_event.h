// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_SCOPED_SENDING_EVENT_H_
#define BASE_MAC_SCOPED_SENDING_EVENT_H_

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/message_loop/message_pump_mac.h"


@protocol CrAppControlProtocol<CrAppProtocol>
- (void)setHandlingSendEvent:(BOOL)handlingSendEvent;
@end

namespace base {
namespace mac {

class BASE_EXPORT ScopedSendingEvent {
 public:
  ScopedSendingEvent();
  ~ScopedSendingEvent();

 private:
  
  
  NSObject<CrAppControlProtocol>* app_;
  BOOL handling_;  

  DISALLOW_COPY_AND_ASSIGN(ScopedSendingEvent);
};

}  
}  

#endif  
