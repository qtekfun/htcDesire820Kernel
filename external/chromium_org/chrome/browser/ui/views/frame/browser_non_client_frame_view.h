// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_NON_CLIENT_FRAME_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_NON_CLIENT_FRAME_VIEW_H_

#include "chrome/browser/ui/views/new_avatar_button.h"
#include "ui/views/window/non_client_view.h"

class AvatarLabel;
class AvatarMenuButton;
class BrowserFrame;
class BrowserView;
class NewAvatarButton;

class BrowserNonClientFrameView : public views::NonClientFrameView {
 public:
  BrowserNonClientFrameView(BrowserFrame* frame, BrowserView* browser_view);
  virtual ~BrowserNonClientFrameView();

  AvatarMenuButton* avatar_button() const { return avatar_button_; }

  NewAvatarButton* new_avatar_button() const { return new_avatar_button_; }

  AvatarLabel* avatar_label() const { return avatar_label_; }

  
  
  virtual gfx::Rect GetBoundsForTabStrip(views::View* tabstrip) const = 0;

  
  
  
  
  virtual int GetTopInset() const = 0;

  
  virtual int GetThemeBackgroundXInset() const = 0;

  
  virtual void UpdateThrobber(bool running) = 0;

  
  virtual void VisibilityChanged(views::View* starting_from,
                                 bool is_visible) OVERRIDE;
  virtual void OnThemeChanged() OVERRIDE;

 protected:
  BrowserView* browser_view() const { return browser_view_; }
  BrowserFrame* frame() const { return frame_; }

  
  void UpdateAvatarInfo();

  
  
  
  void UpdateNewStyleAvatarInfo(views::ButtonListener* listener,
                                const NewAvatarButton::AvatarButtonStyle style);

 private:
  
  BrowserFrame* frame_;

  
  BrowserView* browser_view_;

  
  
  AvatarMenuButton* avatar_button_;

  
  AvatarLabel* avatar_label_;

  
  
  NewAvatarButton* new_avatar_button_;
};

namespace chrome {

BrowserNonClientFrameView* CreateBrowserNonClientFrameView(
    BrowserFrame* frame, BrowserView* browser_view);

}  

#endif  
