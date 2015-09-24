// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_ACCELERATORS_NESTED_DISPATCHER_CONTROLLER_H_
#define ASH_ACCELERATORS_NESTED_DISPATCHER_CONTROLLER_H_

#include "ash/ash_export.h"
#include "base/message_loop/message_loop.h"
#include "ui/aura/client/dispatcher_client.h"
#include "ui/aura/window.h"

namespace ash {

class ASH_EXPORT NestedDispatcherController
    : public aura::client::DispatcherClient {
 public:
  NestedDispatcherController();
  virtual ~NestedDispatcherController();

  virtual void RunWithDispatcher(base::MessageLoop::Dispatcher* dispatcher,
                                 aura::Window* associated_window,
                                 bool nestable_tasks_allowed) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(NestedDispatcherController);
};

}  

#endif  
