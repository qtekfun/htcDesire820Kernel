// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_DELAYABLE_WIDGET_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_DELAYABLE_WIDGET_H_

#include "base/timer/timer.h"
#include "ui/views/widget/widget.h"

namespace chromeos {
namespace input_method {

class DelayableWidget : public views::Widget {
 public:
  DelayableWidget();
  virtual ~DelayableWidget();

  
  virtual void Show() OVERRIDE;

  
  
  
  void DelayShow(unsigned int milliseconds);

  
  void Hide();

  
  
  
  void DelayHide(unsigned int milliseconds);

 private:
  base::OneShotTimer<DelayableWidget> show_hide_timer_;

  DISALLOW_COPY_AND_ASSIGN(DelayableWidget);
};

}  
}  

#endif  
