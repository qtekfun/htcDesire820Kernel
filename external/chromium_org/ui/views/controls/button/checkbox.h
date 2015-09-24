// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_BUTTON_CHECKBOX_H_
#define UI_VIEWS_CONTROLS_BUTTON_CHECKBOX_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "ui/views/controls/button/label_button.h"

namespace views {

class VIEWS_EXPORT Checkbox : public LabelButton {
 public:
  static const char kViewClassName[];

  explicit Checkbox(const string16& label);
  virtual ~Checkbox();

  
  
  void set_listener(ButtonListener* listener) { listener_ = listener; }

  
  virtual void SetChecked(bool checked);
  bool checked() const { return checked_; }

 protected:
  
  virtual void Layout() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual const gfx::ImageSkia& GetImage(ButtonState for_state) OVERRIDE;

  
  
  void SetCustomImage(bool checked,
                      bool focused,
                      ButtonState for_state,
                      const gfx::ImageSkia& image);

 private:
  
  virtual void NotifyClick(const ui::Event& event) OVERRIDE;

  virtual ui::NativeTheme::Part GetThemePart() const OVERRIDE;
  virtual void GetExtraParams(
      ui::NativeTheme::ExtraParams* params) const OVERRIDE;

  
  bool checked_;

  
  gfx::ImageSkia images_[2][2][STATE_COUNT];

  DISALLOW_COPY_AND_ASSIGN(Checkbox);
};

}  

#endif  
