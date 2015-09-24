// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_COMBOBOX_EXAMPLE_H_
#define UI_VIEWS_EXAMPLES_COMBOBOX_EXAMPLE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/base/models/combobox_model.h"
#include "ui/views/controls/combobox/combobox_listener.h"
#include "ui/views/examples/example_base.h"

namespace views {
namespace examples {

class ComboboxModelExample : public ui::ComboboxModel {
 public:
  ComboboxModelExample();
  virtual ~ComboboxModelExample();

  
  virtual int GetItemCount() const OVERRIDE;
  virtual string16 GetItemAt(int index) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(ComboboxModelExample);
};

class ComboboxExample : public ExampleBase, public ComboboxListener {
 public:
  ComboboxExample();
  virtual ~ComboboxExample();

  
  virtual void CreateExampleView(View* container) OVERRIDE;

 private:
  
  virtual void OnSelectedIndexChanged(Combobox* combobox) OVERRIDE;

  ComboboxModelExample combobox_model_;
  Combobox* combobox_;

  DISALLOW_COPY_AND_ASSIGN(ComboboxExample);
};

}  
}  

#endif  
