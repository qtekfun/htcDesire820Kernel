// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_VIEW_H_
#define CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_VIEW_H_

#include "chrome/browser/ui/autofill/autofill_dialog_view_delegate.h"

namespace content {
class NavigationController;
}

namespace gfx {
class Point;
class Size;
}

namespace autofill {

class AutofillDialogViewDelegate;
class TestableAutofillDialogView;

class AutofillDialogView {
 public:
  virtual ~AutofillDialogView();

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  
  
  
  
  virtual void UpdatesStarted() = 0;

  
  virtual void UpdatesFinished() = 0;

  
  virtual void UpdateNotificationArea() = 0;

  
  
  virtual void UpdateAccountChooser() = 0;

  
  virtual void UpdateButtonStrip() = 0;

  
  
  virtual void UpdateOverlay() = 0;

  
  virtual void UpdateDetailArea() = 0;

  
  virtual void UpdateForErrors() = 0;

  
  virtual void UpdateSection(DialogSection section) = 0;

  
  virtual void UpdateErrorBubble() = 0;

  
  
  virtual void FillSection(DialogSection section,
                           const DetailInput& originating_input) = 0;

  
  virtual void GetUserInput(DialogSection section, FieldValueMap* output) = 0;

  
  
  
  virtual base::string16 GetCvc() = 0;

  
  virtual bool HitTestInput(const DetailInput& input,
                            const gfx::Point& screen_point) = 0;

  
  virtual bool SaveDetailsLocally() = 0;

  
  
  virtual const content::NavigationController* ShowSignIn() = 0;

  
  virtual void HideSignIn() = 0;

  
  virtual void ModelChanged() = 0;

  
  
  virtual TestableAutofillDialogView* GetTestableView() = 0;

  
  
  virtual void OnSignInResize(const gfx::Size& pref_size) = 0;

  
  
  static AutofillDialogView* Create(AutofillDialogViewDelegate* delegate);
};

}  

#endif  
