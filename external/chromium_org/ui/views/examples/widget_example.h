// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_WIDGET_EXAMPLE_H_
#define UI_VIEWS_EXAMPLES_WIDGET_EXAMPLE_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/examples/example_base.h"
#include "ui/views/widget/widget.h"

namespace views {
namespace examples {

class WidgetExample : public ExampleBase, public ButtonListener {
 public:
  WidgetExample();
  virtual ~WidgetExample();

  
  virtual void CreateExampleView(View* container) OVERRIDE;

 private:
  
  enum Command {
    POPUP,        
    DIALOG,       
    CHILD,        
    CLOSE_WIDGET, 
  };

  
  void BuildButton(View* container, const std::string& label, int tag);

  
  void ShowWidget(View* sender, Widget::InitParams params);

  
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(WidgetExample);
};

}  
}  

#endif  
