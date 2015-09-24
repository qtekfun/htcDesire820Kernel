// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_TEXT_EXAMPLE_H_
#define UI_VIEWS_EXAMPLES_TEXT_EXAMPLE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_vector.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/combobox/combobox_listener.h"
#include "ui/views/examples/example_base.h"

namespace views {
class Checkbox;
class GridLayout;

namespace examples {

class ExampleComboboxModel;

class TextExample : public ExampleBase,
                    public ButtonListener,
                    public ComboboxListener {
 public:
  TextExample();
  virtual ~TextExample();

  
  virtual void CreateExampleView(View* container) OVERRIDE;

 private:
  
  Checkbox* AddCheckbox(GridLayout* layout, const char* name);

  
  Combobox* AddCombobox(GridLayout* layout,
                        const char* name,
                        const char** strings,
                        int count);

  
  virtual void ButtonPressed(Button* button, const ui::Event& event) OVERRIDE;

  
  virtual void OnSelectedIndexChanged(Combobox* combobox) OVERRIDE;

  class TextExampleView;
  
  TextExampleView* text_view_;

  
  Combobox* h_align_cb_;

  
  Combobox* eliding_cb_;

  
  Combobox* prefix_cb_;

  
  Combobox* text_cb_;

  
  Checkbox* multiline_checkbox_;

  
  Checkbox* break_checkbox_;

  
  Checkbox* halo_checkbox_;

  
  Checkbox* bold_checkbox_;

  
  Checkbox* italic_checkbox_;

  
  Checkbox* underline_checkbox_;

  
  
  ScopedVector<ExampleComboboxModel> example_combobox_model_;

  DISALLOW_COPY_AND_ASSIGN(TextExample);
};

}  
}  

#endif  
