// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_PASSWORDS_MANAGE_PASSWORDS_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_PASSWORDS_MANAGE_PASSWORDS_BUBBLE_VIEW_H_

#include "base/basictypes.h"
#include "chrome/browser/ui/passwords/manage_passwords_bubble_model.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/link.h"
#include "ui/views/controls/link_listener.h"

class ManagePasswordsIconView;

namespace content {
class WebContents;
}

namespace views {
class BlueButton;
class LabelButton;
}

class ManagePasswordsBubbleView : public views::BubbleDelegateView,
                                  public views::ButtonListener,
                                  public views::LinkListener {
 public:
  
  static void ShowBubble(content::WebContents* web_contents,
                         ManagePasswordsIconView* icon_view);

  
  static void CloseBubble();

  
  static bool IsShowing();

 private:
  ManagePasswordsBubbleView(content::WebContents* web_contents,
                            views::View* anchor_view,
                            ManagePasswordsIconView* icon_view);
  virtual ~ManagePasswordsBubbleView();

  
  
  
  int GetMaximumUsernameOrPasswordWidth(bool username);

  
  
  
  
  void AdjustForFullscreen(const gfx::Rect& screen_bounds);

  void Close();

  
  virtual void Init() OVERRIDE;
  virtual void WindowClosing() OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  
  
  
  static ManagePasswordsBubbleView* manage_passwords_bubble_;

  ManagePasswordsBubbleModel* manage_passwords_bubble_model_;
  ManagePasswordsIconView* icon_view_;

  
  views::BlueButton* save_button_;
  views::LabelButton* cancel_button_;
  views::Link* manage_link_;
  views::LabelButton* done_button_;

  DISALLOW_COPY_AND_ASSIGN(ManagePasswordsBubbleView);
};

#endif  
