// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_X11_H
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_X11_H

#include <bitset>
#include <map>

#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_pump.h"
#include "base/message_loop/message_pump_dispatcher.h"
#include "base/message_loop/message_pump_glib.h"
#include "base/message_loop/message_pump_observer.h"
#include "base/observer_list.h"


typedef struct _GPollFD GPollFD;
typedef struct _GSource GSource;
typedef struct _XDisplay Display;

namespace base {

class BASE_EXPORT MessagePumpX11 : public MessagePumpGlib,
                                   public MessagePumpDispatcher {
 public:
  MessagePumpX11();
  virtual ~MessagePumpX11();

  
  static Display* GetDefaultXDisplay();

  
  static MessagePumpX11* Current();

  
  
  void AddDispatcherForWindow(MessagePumpDispatcher* dispatcher,
                              unsigned long xid);
  void RemoveDispatcherForWindow(unsigned long xid);

  
  
  
  void AddDispatcherForRootWindow(MessagePumpDispatcher* dispatcher);
  void RemoveDispatcherForRootWindow(MessagePumpDispatcher* dispatcher);

  
  void AddObserver(MessagePumpObserver* observer);

  
  
  void RemoveObserver(MessagePumpObserver* observer);

  
  
  bool DispatchXEvents();

  
  
  
  
  
  
  
  
  
  void BlockUntilWindowMapped(unsigned long xid);

 private:
  typedef std::map<unsigned long, MessagePumpDispatcher*> DispatchersMap;

  
  void InitXSource();

  
  
  bool ProcessXEvent(MessagePumpDispatcher* dispatcher, XEvent* event);

  
  
  
  bool WillProcessXEvent(XEvent* xevent);
  void DidProcessXEvent(XEvent* xevent);

  
  MessagePumpDispatcher* GetDispatcherForXEvent(const NativeEvent& xev) const;

  ObserverList<MessagePumpObserver>& observers() { return observers_; }

  
  virtual bool Dispatch(const NativeEvent& event) OVERRIDE;

  
  GSource* x_source_;

  
  scoped_ptr<GPollFD> x_poll_;

  DispatchersMap dispatchers_;

  
  
  
  ObserverList<MessagePumpDispatcher> root_window_dispatchers_;

  
  ObserverList<MessagePumpObserver> observers_;

  unsigned long x_root_window_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpX11);
};

#if !defined(TOOLKIT_GTK)
typedef MessagePumpX11 MessagePumpForUI;
#endif

}  

#endif  
