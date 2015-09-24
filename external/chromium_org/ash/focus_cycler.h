// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FOCUS_CYCLER_H_
#define FOCUS_CYCLER_H_

#include <vector>

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"

namespace views {
class Widget;
}  

namespace ash {

namespace internal {

class ASH_EXPORT FocusCycler {
 public:
  enum Direction {
    FORWARD,
    BACKWARD
  };

  FocusCycler();
  ~FocusCycler();

  
  
  const views::Widget* widget_activating() const { return widget_activating_; }

  
  
  void AddWidget(views::Widget* widget);

  
  void RotateFocus(Direction direction);

  
  bool FocusWidget(views::Widget* widget);

 private:
  std::vector<views::Widget*> widgets_;

  
  views::Widget* widget_activating_;

  DISALLOW_COPY_AND_ASSIGN(FocusCycler);
};

}  

}  

#endif  
