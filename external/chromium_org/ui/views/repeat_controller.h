// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_REPEAT_CONTROLLER_H_
#define UI_VIEWS_REPEAT_CONTROLLER_H_

#include "base/callback.h"
#include "base/timer/timer.h"

namespace views {

class RepeatController {
 public:
  
  explicit RepeatController(const base::Closure& callback);
  virtual ~RepeatController();

  
  void Start();

  
  void Stop();

 private:
  
  void Run();

  
  base::OneShotTimer<RepeatController> timer_;

  base::Closure callback_;

  DISALLOW_COPY_AND_ASSIGN(RepeatController);
};

}  

#endif  
