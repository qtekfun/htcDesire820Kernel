// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_SIGN_IN_DELEGATE_H_
#define CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_SIGN_IN_DELEGATE_H_

#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/gfx/size.h"

namespace autofill {

class AutofillDialogView;

class AutofillDialogSignInDelegate: public content::WebContentsDelegate,
                                    public content::WebContentsObserver {
 public:
  AutofillDialogSignInDelegate(AutofillDialogView* dialog_view,
                               content::WebContents* web_contents,
                               content::WebContentsDelegate* wrapped_delegate,
                               const gfx::Size& minimum_size,
                               const gfx::Size& maximum_size);

  
  virtual void ResizeDueToAutoResize(content::WebContents* source,
                                     const gfx::Size& preferred_size) OVERRIDE;
  virtual content::WebContents* OpenURLFromTab(
      content::WebContents* source,
      const content::OpenURLParams& params) OVERRIDE;
  virtual void AddNewContents(content::WebContents* source,
                              content::WebContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture,
                              bool* was_blocked) OVERRIDE;

  
  virtual void RenderViewCreated(
      content::RenderViewHost* render_view_host) OVERRIDE;

  
  void UpdateLimitsAndEnableAutoResize(const gfx::Size& minimum_size,
                                       const gfx::Size& maximum_size);

 private:
  
  
  void EnableAutoResize();

  
  AutofillDialogView* const dialog_view_;

  
  content::WebContentsDelegate* const wrapped_delegate_;

  
  gfx::Size minimum_size_;
  gfx::Size maximum_size_;
};

}  

#endif  
