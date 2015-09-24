// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_TESTABLE_AUTOFILL_DIALOG_VIEW_H_
#define CHROME_BROWSER_UI_AUTOFILL_TESTABLE_AUTOFILL_DIALOG_VIEW_H_

namespace autofill {

class TestableAutofillDialogView {
 public:
  virtual ~TestableAutofillDialogView() {}

  
  virtual void SubmitForTesting() = 0;

  
  virtual void CancelForTesting() = 0;

  
  virtual base::string16 GetTextContentsOfInput(const DetailInput& input) = 0;

  
  virtual void SetTextContentsOfInput(const DetailInput& input,
                                      const base::string16& contents) = 0;

  
  virtual void SetTextContentsOfSuggestionInput(DialogSection section,
                                                const base::string16& text) = 0;

  
  virtual void ActivateInput(const DetailInput& input) = 0;

  
  virtual gfx::Size GetSize() const = 0;

  
  virtual content::WebContents* GetSignInWebContents() = 0;

  
  virtual bool IsShowingOverlay() const = 0;
};

}  

#endif  
