// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_TEXTFIELD_WITH_MARGIN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_TEXTFIELD_WITH_MARGIN_H_
#pragma once

#include "views/controls/textfield/textfield.h"

namespace chromeos {

class TextfieldWithMargin : public views::Textfield {
 public:
  TextfieldWithMargin();
  explicit TextfieldWithMargin(views::Textfield::StyleFlags style);

 protected:
  
  virtual void Layout();
  virtual bool OnKeyPressed(const views::KeyEvent& e);

 private:
  DISALLOW_COPY_AND_ASSIGN(TextfieldWithMargin);
};

}  

#endif  