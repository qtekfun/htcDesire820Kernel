// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_TEXTFIELD_EXAMPLE_H_
#define UI_VIEWS_EXAMPLES_TEXTFIELD_EXAMPLE_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/examples/example_base.h"

namespace views {

class LabelButton;

namespace examples {

class TextfieldExample : public ExampleBase,
                         public TextfieldController,
                         public ButtonListener {
 public:
  TextfieldExample();
  virtual ~TextfieldExample();

  
  virtual void CreateExampleView(View* container) OVERRIDE;

 private:
  
  virtual void ContentsChanged(Textfield* sender,
                               const string16& new_contents) OVERRIDE;
  virtual bool HandleKeyEvent(Textfield* sender,
                              const ui::KeyEvent& key_event) OVERRIDE;
  virtual bool HandleMouseEvent(Textfield* sender,
                                const ui::MouseEvent& mouse_event) OVERRIDE;

  
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

  
  Textfield* name_;
  Textfield* password_;
  Textfield* read_only_;

  
  LabelButton* show_password_;
  LabelButton* clear_all_;
  LabelButton* append_;
  LabelButton* set_;
  LabelButton* set_style_;

  DISALLOW_COPY_AND_ASSIGN(TextfieldExample);
};

}  
}  

#endif  
