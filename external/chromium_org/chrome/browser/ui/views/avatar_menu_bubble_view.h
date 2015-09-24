// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_AVATAR_MENU_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_AVATAR_MENU_BUBBLE_VIEW_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/profiles/avatar_menu_observer.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/link_listener.h"

class AvatarMenu;
class Browser;
class ProfileItemView;

namespace content {
class WebContents;
}

namespace views {
class CustomButton;
class ImageView;
class Label;
class Link;
class Separator;
}

class AvatarMenuBubbleView : public views::BubbleDelegateView,
                             public views::ButtonListener,
                             public views::LinkListener,
                             public AvatarMenuObserver {
 public:
  
  
  
  
  
  
  
  
  
  
  static void ShowBubble(views::View* anchor_view,
                         views::BubbleBorder::Arrow arrow,
                         views::BubbleBorder::BubbleAlignment border_alignment,
                         const gfx::Rect& anchor_rect,
                         Browser* browser);
  static bool IsShowing();
  static void Hide();

  virtual ~AvatarMenuBubbleView();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  
  virtual gfx::Rect GetAnchorRect() OVERRIDE;
  virtual void Init() OVERRIDE;
  virtual void WindowClosing() OVERRIDE;

  
  virtual void OnAvatarMenuChanged(
      AvatarMenu* avatar_menu) OVERRIDE;

  
  
  
  static void clear_close_on_deactivate_for_testing() {
    close_on_deactivate_for_testing_ = false;
  }

 private:
  AvatarMenuBubbleView(views::View* anchor_view,
                       views::BubbleBorder::Arrow arrow,
                       const gfx::Rect& anchor_rect,
                       Browser* browser);

  
  
  
  void SetBackgroundColors();

  
  void InitMenuContents(AvatarMenu* avatar_menu);

  
  void InitManagedUserContents(AvatarMenu* avatar_menu);

  scoped_ptr<AvatarMenu> avatar_menu_;
  gfx::Rect anchor_rect_;
  Browser* browser_;
  std::vector<ProfileItemView*> item_views_;

  
  views::Separator* separator_;

  
  
  
  views::View* buttons_view_;

  
  
  
  views::Label* managed_user_info_;
  views::ImageView* icon_view_;
  views::Separator* separator_switch_users_;
  views::Link* switch_profile_link_;

  static AvatarMenuBubbleView* avatar_bubble_;
  static bool close_on_deactivate_for_testing_;

  
  bool expanded_;

  DISALLOW_COPY_AND_ASSIGN(AvatarMenuBubbleView);
};

#endif  
