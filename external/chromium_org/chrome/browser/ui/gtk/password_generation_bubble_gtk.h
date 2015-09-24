// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_PASSWORD_GENERATION_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_PASSWORD_GENERATION_BUBBLE_GTK_H_

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "chrome/browser/ui/gtk/bubble/bubble_gtk.h"
#include "components/autofill/core/common/password_form.h"
#include "components/autofill/core/common/password_generation_util.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/rect.h"

namespace autofill {
class PasswordGenerator;
}

namespace content {
class WebContents;
}

class PasswordGenerationBubbleGtk : public BubbleDelegateGtk {
 public:
  PasswordGenerationBubbleGtk(const gfx::Rect& anchor_rect,
                              const autofill::PasswordForm& form,
                              content::WebContents* web_contents,
                              autofill::PasswordGenerator* password_generator);
  virtual ~PasswordGenerationBubbleGtk();

  
  virtual void BubbleClosing(BubbleGtk* bubble, bool closed_by_escape) OVERRIDE;

 private:
  CHROMEGTK_CALLBACK_0(PasswordGenerationBubbleGtk, void, OnDestroy);
  CHROMEGTK_CALLBACK_0(PasswordGenerationBubbleGtk, void, OnAcceptClicked);
  CHROMEGTK_CALLBACK_2(PasswordGenerationBubbleGtk, void, OnRegenerateClicked,
                       GtkEntryIconPosition, GdkEvent*);
  CHROMEGTK_CALLBACK_0(PasswordGenerationBubbleGtk, void, OnPasswordEdited);
  CHROMEG_CALLBACK_0(
      PasswordGenerationBubbleGtk, void, OnLearnMoreLinkClicked, GtkButton*);

  BubbleGtk* bubble_;
  GtkWidget* text_field_;

  
  
  autofill::PasswordForm form_;

  
  content::WebContents* web_contents_;

  
  
  autofill::PasswordGenerator* password_generator_;

  
  autofill::password_generation::PasswordGenerationActions actions_;

  DISALLOW_COPY_AND_ASSIGN(PasswordGenerationBubbleGtk);
};

#endif  
