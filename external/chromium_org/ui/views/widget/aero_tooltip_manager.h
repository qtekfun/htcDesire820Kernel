// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_AERO_TOOLTIP_MANAGER_H_
#define UI_VIEWS_WIDGET_AERO_TOOLTIP_MANAGER_H_

#include "base/memory/ref_counted.h"
#include "ui/views/widget/tooltip_manager_win.h"

namespace views {

class AeroTooltipManager : public TooltipManagerWin {
 public:
  explicit AeroTooltipManager(Widget* widget);
  virtual ~AeroTooltipManager();

  virtual void OnMouse(UINT u_msg, WPARAM w_param, LPARAM l_param);

 private:
  void OnTimer();

  class InitialTimer : public base::RefCounted<InitialTimer> {
   public:
    explicit InitialTimer(AeroTooltipManager* manager);
    void Start(int time);
    void Disown();
    void Execute();

   private:
    friend class base::RefCounted<InitialTimer>;

    ~InitialTimer() {}

    AeroTooltipManager* manager_;
  };

  int initial_delay_;
  scoped_refptr<InitialTimer> initial_timer_;
};

}  

#endif  
