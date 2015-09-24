// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_OPAQUE_BROWSER_FRAME_VIEW_LAYOUT_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_OPAQUE_BROWSER_FRAME_VIEW_LAYOUT_H_

#include "chrome/browser/ui/views/frame/opaque_browser_frame_view.h"
#include "ui/views/layout/layout_manager.h"
#include "ui/views/window/frame_buttons.h"

class AvatarMenuButton;
class NewAvatarButton;
class OpaqueBrowserFrameViewLayoutDelegate;

namespace views {
class ImageButton;
class Label;
}

class OpaqueBrowserFrameViewLayout : public views::LayoutManager {
 public:
  explicit OpaqueBrowserFrameViewLayout(
      OpaqueBrowserFrameViewLayoutDelegate* delegate);
  virtual ~OpaqueBrowserFrameViewLayout();

  
  
  
  
  static bool ShouldAddDefaultCaptionButtons();

  
  void SetButtonOrdering(
      const std::vector<views::FrameButton>& leading_buttons,
      const std::vector<views::FrameButton>& trailing_buttons);

  gfx::Rect GetBoundsForTabStrip(
      const gfx::Size& tabstrip_preferred_size,
      int available_width) const;

  gfx::Size GetMinimumSize(int available_width) const;

  
  
  gfx::Rect GetWindowBoundsForClientBounds(
      const gfx::Rect& client_bounds) const;

  
  
  
  int FrameBorderThickness(bool restored) const;

  
  
  int NonClientBorderThickness() const;

  
  
  
  int NonClientTopBorderHeight(bool restored) const;

  int GetTabStripInsetsTop(bool restored) const;

  
  
  int CaptionButtonY(bool restored) const;

  
  
  
  int TitlebarBottomThickness(bool restored) const;

  
  
  gfx::Rect IconBounds() const;

  
  gfx::Rect CalculateClientAreaBounds(int width, int height) const;

  void set_extra_caption_y(int extra_caption_y) {
    extra_caption_y_ = extra_caption_y;
  }

  void set_window_caption_spacing(int window_caption_spacing) {
    window_caption_spacing_ = window_caption_spacing;
  }

  const gfx::Rect& client_view_bounds() const { return client_view_bounds_; }

 private:
  
  
  enum ButtonAlignment {
    ALIGN_LEADING,
    ALIGN_TRAILING
  };

  
  
  bool ShouldAvatarBeOnRight() const;

  
  void LayoutWindowControls(views::View* host);
  void LayoutTitleBar(views::View* host);
  void LayoutAvatar(views::View* host);
  void LayoutNewStyleAvatar(views::View* host);

  void ConfigureButton(views::View* host,
                       views::FrameButton button_id,
                       ButtonAlignment align,
                       int caption_y);

  
  void HideButton(views::FrameButton button_id);

  
  void SetBoundsForButton(views::View* host,
                          views::ImageButton* button,
                          ButtonAlignment align,
                          int caption_y);

  
  void SetView(int id, views::View* view);

  
  virtual void Layout(views::View* host) OVERRIDE;
  virtual gfx::Size GetPreferredSize(views::View* host) OVERRIDE;
  virtual void ViewAdded(views::View* host, views::View* view) OVERRIDE;
  virtual void ViewRemoved(views::View* host, views::View* view) OVERRIDE;

  OpaqueBrowserFrameViewLayoutDelegate* delegate_;

  
  gfx::Rect avatar_bounds_;

  
  gfx::Rect client_view_bounds_;

  
  gfx::Rect window_icon_bounds_;

  
  
  int leading_button_start_;
  int trailing_button_start_;

  
  
  
  int minimum_size_for_buttons_;

  
  bool has_leading_buttons_;
  bool has_trailing_buttons_;

  
  
  int extra_caption_y_;

  
  int window_caption_spacing_;

  
  views::ImageButton* minimize_button_;
  views::ImageButton* maximize_button_;
  views::ImageButton* restore_button_;
  views::ImageButton* close_button_;

  views::View* window_icon_;
  views::Label* window_title_;

  views::View* avatar_label_;
  AvatarMenuButton* avatar_button_;
  views::View* new_avatar_button_;

  std::vector<views::FrameButton> leading_buttons_;
  std::vector<views::FrameButton> trailing_buttons_;

  DISALLOW_COPY_AND_ASSIGN(OpaqueBrowserFrameViewLayout);
};

#endif  
