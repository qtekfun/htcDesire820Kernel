// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_AUTOFILL_DECORATED_TEXTFIELD_H_
#define CHROME_BROWSER_UI_VIEWS_AUTOFILL_DECORATED_TEXTFIELD_H_

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "ui/gfx/image/image.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/controls/textfield/textfield.h"

namespace views {
class FocusableBorder;
class TextfieldController;
}

namespace autofill {

class DecoratedTextfield : public views::Textfield {
 public:
  static const char kViewClassName[];

  DecoratedTextfield(const base::string16& default_value,
                     const base::string16& placeholder,
                     views::TextfieldController* controller);
  virtual ~DecoratedTextfield();

  
  void SetInvalid(bool invalid);
  bool invalid() const { return invalid_; }

  
  void SetEditable(bool editable);
  bool editable() const { return editable_; }

  
  
  void SetIcon(const gfx::Image& icon);

  
  
  void SetTooltipIcon(const base::string16& text);

  
  virtual base::string16 GetPlaceholderText() const OVERRIDE;

  
  virtual const char* GetClassName() const OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual views::View* GetEventHandlerForRect(const gfx::Rect& rect) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(DecoratedTextfieldTest, HeightMatchesButton);

  
  
  void UpdateBackground();

  
  
  void IconChanged();

  
  
  
  
  static const int kMagicInsetNumber;

  
  views::FocusableBorder* border_;  

  
  scoped_ptr<views::ImageView> icon_view_;

  
  
  bool invalid_;

  
  
  
  bool editable_;

  DISALLOW_COPY_AND_ASSIGN(DecoratedTextfield);
};

}  

#endif  
