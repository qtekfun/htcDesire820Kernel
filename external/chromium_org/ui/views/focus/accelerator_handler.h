// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_FOCUS_ACCELERATOR_HANDLER_H_
#define UI_VIEWS_FOCUS_ACCELERATOR_HANDLER_H_

#include "build/build_config.h"

#include <set>
#include <vector>

#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "ui/views/views_export.h"

namespace views {

#if defined(USE_AURA) && defined(USE_X11)
bool VIEWS_EXPORT DispatchXEvent(XEvent* xevent);
#endif  

class VIEWS_EXPORT AcceleratorHandler : public base::MessageLoop::Dispatcher {
 public:
  AcceleratorHandler();

  
  
  virtual bool Dispatch(const base::NativeEvent& event) OVERRIDE;

 private:
#if defined(OS_WIN)
  
  std::set<WPARAM> pressed_keys_;
#endif

  DISALLOW_COPY_AND_ASSIGN(AcceleratorHandler);
};

}  

#endif  
