// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_NATIVE_WIDGET_H_
#define UI_VIEWS_WIDGET_NATIVE_WIDGET_H_

#include "ui/views/widget/widget.h"

namespace ui {
class EventHandler;
}

namespace views {
namespace internal {
class NativeWidgetPrivate;
}

class VIEWS_EXPORT NativeWidget {
 public:
  virtual ~NativeWidget() {}

  
  virtual ui::EventHandler* GetEventHandler() = 0;

 private:
  friend class Widget;

  virtual internal::NativeWidgetPrivate* AsNativeWidgetPrivate() = 0;
};

}  

#endif  
