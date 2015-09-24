// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_BUTTON_RADIO_BUTTON_H_
#define UI_VIEWS_CONTROLS_BUTTON_RADIO_BUTTON_H_

#include "base/strings/string16.h"
#include "ui/views/controls/button/checkbox.h"

namespace views {

class VIEWS_EXPORT RadioButton : public Checkbox {
 public:
  
  static const char kViewClassName[];

  RadioButton(const string16& label, int group_id);
  virtual ~RadioButton();

  
  virtual const char* GetClassName() const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual View* GetSelectedViewForGroup(int group) OVERRIDE;
  virtual bool IsGroupFocusTraversable() const OVERRIDE;
  virtual void OnFocus() OVERRIDE;

  
  virtual void NotifyClick(const ui::Event& event) OVERRIDE;

  
  virtual ui::NativeTheme::Part GetThemePart() const OVERRIDE;

  
  virtual void SetChecked(bool checked) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(RadioButton);
};

}  

#endif  
