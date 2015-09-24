// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_POPUP_VIEW_BRIDGE_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_POPUP_VIEW_BRIDGE_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/autofill/autofill_popup_view.h"

@class AutofillPopupViewCocoa;
@class NSWindow;

namespace autofill {

class AutofillPopupController;

class AutofillPopupViewBridge : public AutofillPopupView {
 public:
  explicit AutofillPopupViewBridge(AutofillPopupController* controller);

 private:
  virtual ~AutofillPopupViewBridge();

  
  virtual void Hide() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void InvalidateRow(size_t row) OVERRIDE;
  virtual void UpdateBoundsAndRedrawPopup() OVERRIDE;

  
  
  void SetInitialBounds();

  
  AutofillPopupController* controller_;  

  
  NSWindow* window_;  
  AutofillPopupViewCocoa* view_;  

  DISALLOW_COPY_AND_ASSIGN(AutofillPopupViewBridge);
};

}  

#endif  
