// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_ACCELERATORS_EXIT_WARNING_HANDLER_H_
#define ASH_ACCELERATORS_EXIT_WARNING_HANDLER_H_

#include "ash/ash_export.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "ui/base/accelerators/accelerator.h"

namespace views {
class Widget;
}

namespace ash {


class AcceleratorControllerTest;

class ASH_EXPORT ExitWarningHandler {
 public:
  ExitWarningHandler();

  ~ExitWarningHandler();

  
  void HandleAccelerator();

 private:
  friend class AcceleratorControllerTest;

  enum State {
    IDLE,
    WAIT_FOR_DOUBLE_PRESS,
    EXITING
  };

  
  void TimerAction();

  void StartTimer();
  void CancelTimer();

  void Show();
  void Hide();

  State state_;
  scoped_ptr<views::Widget> widget_;
  base::OneShotTimer<ExitWarningHandler> timer_;

  
  
  bool stub_timer_for_test_;

  DISALLOW_COPY_AND_ASSIGN(ExitWarningHandler);
};

}  

#endif  
