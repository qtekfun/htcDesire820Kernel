// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_BUTTON_EXAMPLE_H_
#define UI_VIEWS_EXAMPLES_BUTTON_EXAMPLE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/controls/button/text_button.h"
#include "ui/views/examples/example_base.h"

namespace views {

class ImageButton;
class LabelButton;

namespace examples {

class ButtonExample : public ExampleBase, public ButtonListener {
 public:
  ButtonExample();
  virtual ~ButtonExample();

  
  virtual void CreateExampleView(View* container) OVERRIDE;

 private:
  void TextButtonPressed(const ui::Event& event);
  void LabelButtonPressed(const ui::Event& event);

  
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

  
  TextButton* text_button_;
  LabelButton* label_button_;
  ImageButton* image_button_;

  
  TextButton::TextAlignment alignment_;
  bool use_native_theme_border_;
  const gfx::ImageSkia* icon_;

  
  int count_;

  DISALLOW_COPY_AND_ASSIGN(ButtonExample);
};

}  
}  

#endif  
