// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_PASSWORD_GENERATION_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_PASSWORD_GENERATION_BUBBLE_VIEW_H_

#include "base/basictypes.h"
#include "components/autofill/core/common/password_form.h"
#include "components/autofill/core/common/password_generation_util.h"
#include "ui/gfx/rect.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/view.h"

namespace autofill {
class PasswordGenerator;
}

namespace content {
class PageNavigator;
class RenderViewHost;
}

namespace views {
class ImageButton;
class Label;
class LabelButton;
}

class PasswordManager;

class PasswordGenerationBubbleView : public views::BubbleDelegateView,
                                     public views::ButtonListener,
                                     public views::TextfieldController {
 public:
  PasswordGenerationBubbleView(const autofill::PasswordForm& form,
                               const gfx::Rect& anchor_rect,
                               views::View* anchor_view,
                               content::RenderViewHost* render_view_host,
                               PasswordManager* password_manager,
                               autofill::PasswordGenerator* password_generator,
                               content::PageNavigator* navigator,
                               ui::ThemeProvider* theme_provider);
  virtual ~PasswordGenerationBubbleView();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;

 private:
  
  virtual void Init() OVERRIDE;
  virtual gfx::Rect GetAnchorRect() OVERRIDE;
  virtual void WindowClosing() OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void ContentsChanged(views::Textfield* sender,
                               const base::string16& new_contents) OVERRIDE;
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const ui::KeyEvent& key_event) OVERRIDE;

  
  virtual views::View* GetInitiallyFocusedView() OVERRIDE;

  
  views::Label* title_label_;
  views::LabelButton* accept_button_;
  views::Textfield* textfield_;
  views::ImageButton* regenerate_button_;
  views::View* textfield_wrapper_;

  
  autofill::PasswordForm form_;

  
  gfx::Rect anchor_rect_;

  
  content::RenderViewHost* render_view_host_;

  
  PasswordManager* password_manager_;

  
  autofill::PasswordGenerator* password_generator_;

  
  
  content::PageNavigator* navigator_;

  
  ui::ThemeProvider* theme_provider_;

  
  autofill::password_generation::PasswordGenerationActions actions_;

  DISALLOW_COPY_AND_ASSIGN(PasswordGenerationBubbleView);
};

#endif  
