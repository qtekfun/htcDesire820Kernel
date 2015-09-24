// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_OZONE_EVENT_FACTORY_OZONE_H_
#define UI_EVENTS_OZONE_EVENT_FACTORY_OZONE_H_

#include <map>

#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_pump_libevent.h"
#include "ui/events/events_export.h"
#include "ui/events/ozone/event_converter_ozone.h"

namespace ui {

class EVENTS_EXPORT EventFactoryOzone {
 public:
  EventFactoryOzone();
  virtual ~EventFactoryOzone();

  
  
  
  
  
  
  virtual void StartProcessingEvents();

  
  static EventFactoryOzone* GetInstance();

  
  static void SetInstance(EventFactoryOzone*);

  
  
  void AddEventConverter(int fd, scoped_ptr<EventConverterOzone> converter);

  
  void RemoveEventConverter(int fd);

 private:
  
  typedef EventConverterOzone* Converter;
  typedef base::MessagePumpLibevent::FileDescriptorWatcher* FDWatcher;
  std::map<int, Converter> converters_;
  std::map<int, FDWatcher> watchers_;

  static EventFactoryOzone* impl_;  

  DISALLOW_COPY_AND_ASSIGN(EventFactoryOzone);
};

}  

#endif  
