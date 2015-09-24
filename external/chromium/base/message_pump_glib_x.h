// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_PUMP_GLIB_X_H
#define BASE_MESSAGE_PUMP_GLIB_X_H

#include "base/message_pump.h"
#include "base/message_pump_glib.h"

#include <bitset>
#include <set>

#include <glib.h>
#include <gtk/gtk.h>
#include <X11/X.h>

namespace base {

class MessagePumpGlibX : public MessagePumpForUI {
 public:
  MessagePumpGlibX();
  virtual ~MessagePumpGlibX();

  
  
  bool IsDispatchingEvent(void) { return dispatching_event_; }

#if defined(HAVE_XINPUT2)
  
  void SetupXInput2ForXWindow(Window xid);
#endif

  
  virtual bool RunOnce(GMainContext* context, bool block);

 private:
  static void EventDispatcherX(GdkEvent* event, gpointer data);

  
  
  void InitializeEventsToCapture(void);

#if defined(HAVE_XINPUT2)
  
  void InitializeXInput2(void);

  
  int xiopcode_;

  
  
  
  std::set<int> pointer_devices_;
#endif

  
  GSource* gdksource_;

  
  
  gboolean (*gdkdispatcher_)(GSource*, GSourceFunc, void*);

  
  
  bool dispatching_event_;

#if ! GTK_CHECK_VERSION(2,18,0)
#define GDK_EVENT_LAST 37
#endif

  
  
  
  std::bitset<LASTEvent> capture_x_events_;
  std::bitset<GDK_EVENT_LAST> capture_gdk_events_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpGlibX);
};

}  

#endif  
