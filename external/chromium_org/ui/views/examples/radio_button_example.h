// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_RADIO_BUTTON_EXAMPLE_H_
#define UI_VIEWS_EXAMPLES_RADIO_BUTTON_EXAMPLE_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/examples/example_base.h"

namespace views {

class LabelButton;
class RadioButton;

namespace examples {

class RadioButtonExample : public ExampleBase, public ButtonListener {
 public:
  RadioButtonExample();
  virtual ~RadioButtonExample();

  
  virtual void CreateExampleView(View* container) OVERRIDE;

 private:
  
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

  
  RadioButton* radio_buttons_[3];

  
  LabelButton* select_;
  LabelButton* status_;

  
  int count_;

  DISALLOW_COPY_AND_ASSIGN(RadioButtonExample);
};

}  
}  

#endif  
