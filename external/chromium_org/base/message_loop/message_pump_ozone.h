// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_OZONE_H_
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_OZONE_H_

#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/message_loop/message_pump_dispatcher.h"
#include "base/message_loop/message_pump_libevent.h"
#include "base/message_loop/message_pump_observer.h"
#include "base/observer_list.h"

namespace base {

class BASE_EXPORT MessagePumpOzone : public MessagePumpLibevent,
                                     public MessagePumpDispatcher {
 public:
  MessagePumpOzone();
  virtual ~MessagePumpOzone();

  
  static MessagePumpOzone* Current();

  
  void AddDispatcherForRootWindow(MessagePumpDispatcher* dispatcher);
  void RemoveDispatcherForRootWindow(MessagePumpDispatcher* dispatcher);

  void RunWithDispatcher(Delegate* delegate, MessagePumpDispatcher* dispatcher);

  
  
  void AddObserver(MessagePumpObserver* observer);
  void RemoveObserver(MessagePumpObserver* observer);

  
  virtual bool Dispatch(const NativeEvent& event) OVERRIDE;

 private:
  std::vector<MessagePumpDispatcher*> dispatcher_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpOzone);
};

typedef MessagePumpOzone MessagePumpForUI;

}  

#endif  
