// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_POPUP_DELEGATE_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_POPUP_DELEGATE_H_

#include "base/strings/string16.h"

namespace ui {
class MouseEvent;
}

namespace autofill {

class AutofillPopupDelegate {
 public:
  
  virtual void OnPopupShown() = 0;

  
  virtual void OnPopupHidden() = 0;

  
  
  virtual bool ShouldRepostEvent(const ui::MouseEvent& event) = 0;

  
  
  virtual void DidSelectSuggestion(int identifier) = 0;

  
  virtual void DidAcceptSuggestion(const base::string16& value,
                                   int identifier) = 0;

  
  virtual void RemoveSuggestion(const base::string16& value,
                                int identifier) = 0;

  
  virtual void ClearPreviewedForm() = 0;
};

}  

#endif  
