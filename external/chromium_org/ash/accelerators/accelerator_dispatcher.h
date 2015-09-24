// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_ACCELERATORS_ACCELERATOR_DISPATCHER_H_
#define ASH_ACCELERATORS_ACCELERATOR_DISPATCHER_H_

#include "ash/ash_export.h"
#include "base/message_loop/message_loop.h"
#include "ui/aura/window.h"
#include "ui/aura/window_observer.h"

namespace ash {

class ASH_EXPORT AcceleratorDispatcher : public base::MessageLoop::Dispatcher,
                                         public aura::WindowObserver {
 public:
  AcceleratorDispatcher(base::MessageLoop::Dispatcher* nested_dispatcher,
                        aura::Window* associated_window);
  virtual ~AcceleratorDispatcher();

  
  virtual bool Dispatch(const base::NativeEvent& event) OVERRIDE;

  
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;

 private:
  base::MessageLoop::Dispatcher* nested_dispatcher_;

  
  
  aura::Window* associated_window_;

  DISALLOW_COPY_AND_ASSIGN(AcceleratorDispatcher);
};

}  

#endif  
