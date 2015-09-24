// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_TABBED_PANE_EXAMPLE_H_
#define UI_VIEWS_EXAMPLES_TABBED_PANE_EXAMPLE_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/tabbed_pane/tabbed_pane_listener.h"
#include "ui/views/examples/example_base.h"

namespace views {
class TabbedPane;

namespace examples {

class TabbedPaneExample : public ExampleBase,
                          public ButtonListener,
                          public TabbedPaneListener {
 public:
  TabbedPaneExample();
  virtual ~TabbedPaneExample();

  
  virtual void CreateExampleView(View* container) OVERRIDE;

 private:
  
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

  
  virtual void TabSelectedAt(int index) OVERRIDE;

  
  void PrintStatus();

  void AddButton(const std::string& label);

  
  TabbedPane* tabbed_pane_;

  
  Button* add_;
  Button* add_at_;
  Button* select_at_;

  DISALLOW_COPY_AND_ASSIGN(TabbedPaneExample);
};

}  
}  

#endif  
