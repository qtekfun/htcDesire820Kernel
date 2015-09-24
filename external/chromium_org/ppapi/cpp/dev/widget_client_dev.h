// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_WIDGET_CLIENT_DEV_H_
#define PPAPI_CPP_DEV_WIDGET_CLIENT_DEV_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/cpp/instance_handle.h"

namespace pp {

class Instance;
class Rect;
class Scrollbar_Dev;
class Widget_Dev;

class WidgetClient_Dev {
 public:
  explicit WidgetClient_Dev(Instance* instance);
  virtual ~WidgetClient_Dev();

  virtual void InvalidateWidget(Widget_Dev widget, const Rect& dirty_rect) = 0;

  virtual void ScrollbarValueChanged(Scrollbar_Dev scrollbar,
                                     uint32_t value) = 0;

  virtual void ScrollbarOverlayChanged(Scrollbar_Dev scrollbar,
                                       bool type) = 0;

 private:
  InstanceHandle associated_instance_;
};

}  

#endif  
