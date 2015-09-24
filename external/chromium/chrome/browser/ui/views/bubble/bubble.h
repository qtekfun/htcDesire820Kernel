// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BUBBLE_BUBBLE_H_
#define CHROME_BROWSER_UI_VIEWS_BUBBLE_BUBBLE_H_
#pragma once

#include "chrome/browser/ui/views/bubble/bubble_border.h"
#include "ui/base/animation/animation_delegate.h"
#include "views/accelerator.h"
#include "views/view.h"

#if defined(OS_WIN)
#include "views/widget/widget_win.h"
#elif defined(OS_LINUX)
#include "views/widget/widget_gtk.h"
#endif


class BorderContents;
#if defined(OS_WIN)
class BorderWidgetWin;
#endif
class Bubble;

namespace gfx {
class Path;
}

namespace ui {
class SlideAnimation;
}

namespace views {
class Widget;
}

class BubbleDelegate {
 public:
  
  
  
  virtual void BubbleClosing(Bubble* bubble, bool closed_by_escape) = 0;

  
  virtual bool CloseOnEscape() = 0;

  
  
  
  
  
  
  virtual bool FadeInOnShow() = 0;

  
  virtual std::wstring accessible_name();
};

class Bubble
#if defined(OS_WIN)
    : public views::WidgetWin,
#elif defined(OS_LINUX)
    : public views::WidgetGtk,
#endif
      public views::AcceleratorTarget,
      public ui::AnimationDelegate {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  static Bubble* Show(views::Widget* parent,
                      const gfx::Rect& position_relative_to,
                      BubbleBorder::ArrowLocation arrow_location,
                      views::View* contents,
                      BubbleDelegate* delegate);

#if defined(OS_CHROMEOS)
  
  
  
  
  
  static Bubble* ShowFocusless(views::Widget* parent,
                               const gfx::Rect& position_relative_to,
                               BubbleBorder::ArrowLocation arrow_location,
                               views::View* contents,
                               BubbleDelegate* delegate,
                               bool show_while_screen_is_locked);
#endif

  
  
  void SizeToContents();

  
  
  
  
  void set_fade_away_on_close(bool fade_away_on_close) {
    fade_away_on_close_ = fade_away_on_close;
  }

  
  virtual void Close();

  
  virtual void AnimationEnded(const ui::Animation* animation);
  virtual void AnimationProgressed(const ui::Animation* animation);

  static const SkColor kBackgroundColor;

 protected:
  Bubble();
#if defined(OS_CHROMEOS)
  Bubble(views::WidgetGtk::Type type, bool show_while_screen_is_locked);
#endif
  virtual ~Bubble();

  
  virtual void InitBubble(views::Widget* parent,
                          const gfx::Rect& position_relative_to,
                          BubbleBorder::ArrowLocation arrow_location,
                          views::View* contents,
                          BubbleDelegate* delegate);

  
  
  virtual BorderContents* CreateBorderContents();

#if defined(OS_WIN)
  
  virtual void OnActivate(UINT action, BOOL minimized, HWND window);
#elif defined(OS_LINUX)
  
  virtual void IsActiveChanged();
#endif

#if defined(OS_WIN)
  
  BorderWidgetWin* border_;
#elif defined(OS_LINUX)
  
  BorderContents* border_contents_;
#endif

 private:
  enum ShowStatus {
    kOpen,
    kClosing,
    kClosed
  };

  
  
  void DoClose(bool closed_by_escape);

  
  void FadeIn();
  
  void FadeOut();

  
  void Fade(bool fade_in);

  
  virtual bool AcceleratorPressed(const views::Accelerator& accelerator);

  
  BubbleDelegate* delegate_;

  
  scoped_ptr<ui::SlideAnimation> animation_;

  
  ShowStatus show_status_;

  
  bool fade_away_on_close_;

#if defined(OS_CHROMEOS)
  
  
  bool show_while_screen_is_locked_;
#endif

  gfx::Rect position_relative_to_;
  BubbleBorder::ArrowLocation arrow_location_;

  views::View* contents_;

  DISALLOW_COPY_AND_ASSIGN(Bubble);
};

#endif  
