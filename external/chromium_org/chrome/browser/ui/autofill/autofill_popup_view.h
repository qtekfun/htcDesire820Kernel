// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_POPUP_VIEW_H_
#define CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_POPUP_VIEW_H_

#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Rect;
}

namespace ui {
class KeyEvent;
}

namespace autofill {

class AutofillPopupController;

class AutofillPopupView {
 public:
  
  static const int kBorderThickness = 1;

  
  
  static const size_t kNamePadding = 15;

  
  static const int kIconPadding = 5;

  
  static const int kEndPadding = 3;

  
  static const int kDeleteIconHeight = 16;

  
  static const int kDeleteIconWidth = 16;

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  
  virtual void InvalidateRow(size_t row) = 0;

  
  virtual void UpdateBoundsAndRedrawPopup() = 0;

  
  static AutofillPopupView* Create(AutofillPopupController* controller);

 protected:
  virtual ~AutofillPopupView() {}
};

}  

#endif  
