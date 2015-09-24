// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_OZONE_EVENT_CONVERTER_OZONE_H_
#define UI_EVENTS_OZONE_EVENT_CONVERTER_OZONE_H_

#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_pump_libevent.h"
#include "ui/events/events_export.h"

namespace ui {
class Event;

class EVENTS_EXPORT EventConverterOzone
    : public base::MessagePumpLibevent::Watcher {
 public:
  EventConverterOzone();
  virtual ~EventConverterOzone();

 protected:
  
  
  
  virtual void DispatchEvent(scoped_ptr<ui::Event> event);

 private:
  DISALLOW_COPY_AND_ASSIGN(EventConverterOzone);
};

}  

#endif  
