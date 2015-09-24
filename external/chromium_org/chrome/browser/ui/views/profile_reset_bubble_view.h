// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_PROFILE_RESET_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_PROFILE_RESET_BUBBLE_VIEW_H_

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/global_error/global_error_bubble_view_base.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/link_listener.h"

namespace content {
class PageNavigator;
}

namespace views {
class Checkbox;
class ImageButton;
class LabelButton;
class Link;
}

class Browser;
class Profile;
class ProfileResetGlobalError;

class ProfileResetBubbleView : public views::BubbleDelegateView,
                               public views::ButtonListener,
                               public views::LinkListener,
                               public GlobalErrorBubbleViewBase {
 public:
  static ProfileResetBubbleView* ShowBubble(
      const base::WeakPtr<ProfileResetGlobalError>& global_error,
      Browser* browser);

  
  virtual views::View* GetInitiallyFocusedView() OVERRIDE;
  virtual void Init() OVERRIDE;

  
  virtual void WindowClosing() OVERRIDE;

  
  virtual void CloseBubbleView() OVERRIDE;

 private:
  ProfileResetBubbleView(
      const base::WeakPtr<ProfileResetGlobalError>& global_error,
      views::View* anchor_view,
      content::PageNavigator* navigator,
      Profile* profile);

  virtual ~ProfileResetBubbleView();

  
  void ResetAllChildren();

  
  
  void SetupLayoutManager(bool report_checked);

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  struct Controls {
    Controls() {
      Reset();
    }
    void Reset() {
      reset_button = NULL;
      no_thanks_button = NULL;
      help_button = NULL;
      report_settings_checkbox = NULL;
    }

    
    views::LabelButton* reset_button;

    
    views::LabelButton* no_thanks_button;

    
    views::ImageButton* help_button;

    
    views::Checkbox* report_settings_checkbox;
  } controls_;

  
  gfx::ImageSkia brighter_help_image_;

  
  content::PageNavigator* navigator_;

  
  
  Profile* profile_;

  
  base::WeakPtr<ProfileResetGlobalError> global_error_;

  
  bool resetting_;

  
  bool chose_to_reset_;

  
  
  bool show_help_pane_;

  
  base::WeakPtrFactory<ProfileResetBubbleView> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ProfileResetBubbleView);
};

#endif  
