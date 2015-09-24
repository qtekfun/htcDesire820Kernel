// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_VIEWS_NOTIFICATIONS_BALLOON_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_NOTIFICATIONS_BALLOON_VIEW_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "chrome/browser/notifications/balloon.h"
#include "chrome/browser/ui/views/notifications/balloon_view_host.h"
#include "content/common/notification_registrar.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/gfx/path.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "views/controls/button/menu_button.h"
#include "views/controls/label.h"
#include "views/controls/menu/view_menu_delegate.h"
#include "views/view.h"
#include "views/widget/widget_delegate.h"

namespace views {
class ButtonListener;
class ImageButton;
class ImagePainter;
class TextButton;
class WidgetWin;
class Menu2;
}  

class BalloonCollection;
class NotificationDetails;
class NotificationOptionsMenuModel;
class NotificationSource;

namespace ui {
class SlideAnimation;
}

class BalloonViewImpl : public BalloonView,
                        public views::View,
                        public views::ViewMenuDelegate,
                        public views::WidgetDelegate,
                        public views::ButtonListener,
                        public NotificationObserver,
                        public ui::AnimationDelegate {
 public:
  explicit BalloonViewImpl(BalloonCollection* collection);
  ~BalloonViewImpl();

  
  virtual void Show(Balloon* balloon) OVERRIDE;
  virtual void Update() OVERRIDE;
  virtual void RepositionToBalloon() OVERRIDE;
  virtual void Close(bool by_user) OVERRIDE;
  virtual gfx::Size GetSize() const OVERRIDE;
  virtual BalloonHost* GetHost() const OVERRIDE;

 private:
  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  virtual void RunMenu(views::View* source, const gfx::Point& pt) OVERRIDE;

  
  virtual void OnDisplayChanged() OVERRIDE;
  virtual void OnWorkAreaChanged() OVERRIDE;

  
  virtual void ButtonPressed(
      views::Button* sender, const views::Event&) OVERRIDE;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  virtual void AnimationProgressed(const ui::Animation* animation) OVERRIDE;

  
  void RunOptionsMenu(const gfx::Point& pt);

  
  void CreateOptionsMenu();

  
  void GetContentsMask(const gfx::Rect& contents_rect, gfx::Path* path) const;

  
  void GetFrameMask(const gfx::Rect&, gfx::Path* path) const;

  
  void SizeContentsWindow();

  
  void DelayedClose(bool by_user);

  
  
  int GetShelfHeight() const;

  
  int GetBalloonFrameHeight() const;

  int GetTotalWidth() const;
  int GetTotalHeight() const;

  gfx::Rect GetCloseButtonBounds() const;
  gfx::Rect GetOptionsButtonBounds() const;
  gfx::Rect GetLabelBounds() const;

  
  
  gfx::Point GetContentsOffset() const;

  
  gfx::Rect GetContentsRectangle() const;

  
  Balloon* balloon_;

  
  BalloonCollection* collection_;

  
  
  views::Widget* frame_container_;

  
  
  views::Widget* html_container_;

  
  scoped_ptr<BalloonViewHost> html_contents_;

  
  ScopedRunnableMethodFactory<BalloonViewImpl> method_factory_;

  
  views::ImageButton* close_button_;

  
  views::Label* source_label_;

  
  scoped_ptr<ui::SlideAnimation> animation_;
  gfx::Rect anim_frame_start_;
  gfx::Rect anim_frame_end_;

  
  scoped_ptr<NotificationOptionsMenuModel> options_menu_model_;
  scoped_ptr<views::Menu2> options_menu_menu_;
  views::MenuButton* options_menu_button_;

  NotificationRegistrar notification_registrar_;

  DISALLOW_COPY_AND_ASSIGN(BalloonViewImpl);
};

#endif  
