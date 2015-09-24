// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_GTK_H_
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_GTK_H_

#include "base/message_loop/message_pump_glib.h"

typedef union _GdkEvent GdkEvent;
typedef struct _XDisplay Display;

namespace base {

class MessagePumpGdkObserver {
 public:
  
  virtual void WillProcessEvent(GdkEvent* event) = 0;

  
  virtual void DidProcessEvent(GdkEvent* event) = 0;

 protected:
  virtual ~MessagePumpGdkObserver() {}
};

class BASE_EXPORT MessagePumpGtk : public MessagePumpGlib {
 public:
  MessagePumpGtk();
  virtual ~MessagePumpGtk();

  
  
  void DispatchEvents(GdkEvent* event);

  
  static Display* GetDefaultXDisplay();

  
  void AddObserver(MessagePumpGdkObserver* observer);

  
  
  void RemoveObserver(MessagePumpGdkObserver* observer);

 private:
  
  
  void WillProcessEvent(GdkEvent* event);

  
  
  void DidProcessEvent(GdkEvent* event);

  
  static void EventDispatcher(GdkEvent* event, void* data);

  
  ObserverList<MessagePumpGdkObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpGtk);
};

typedef MessagePumpGtk MessagePumpForUI;

}  

#endif  
