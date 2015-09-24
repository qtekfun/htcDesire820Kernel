// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_NOTIFICATIONS_BALLOON_VIEW_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_NOTIFICATIONS_BALLOON_VIEW_VIEWS_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/notifications/balloon.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/rect.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/button/menu_button_listener.h"
#include "ui/views/widget/widget_delegate.h"

class BalloonCollection;
class BalloonViewHost;
class NotificationOptionsMenuModel;

namespace gfx {
class Path;
class SlideAnimation;
}

namespace views {
class ImageButton;
class Label;
class MenuButton;
class MenuRunner;
}

class BalloonViewImpl : public BalloonView,
                        public views::MenuButtonListener,
                        public views::WidgetDelegateView,
                        public views::ButtonListener,
                        public content::NotificationObserver,
                        public gfx::AnimationDelegate {
 public:
  explicit BalloonViewImpl(BalloonCollection* collection);
  virtual ~BalloonViewImpl();

  
  virtual void Show(Balloon* balloon) OVERRIDE;
  virtual void Update() OVERRIDE;
  virtual void RepositionToBalloon() OVERRIDE;
  virtual void Close(bool by_user) OVERRIDE;
  virtual gfx::Size GetSize() const OVERRIDE;
  virtual BalloonHost* GetHost() const OVERRIDE;

  void set_enable_web_ui(bool enable) { enable_web_ui_ = enable; }

 private:
  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  virtual void OnMenuButtonClicked(views::View* source,
                                   const gfx::Point& point) OVERRIDE;

  
  virtual void OnDisplayChanged() OVERRIDE;
  virtual void OnWorkAreaChanged() OVERRIDE;
  virtual void DeleteDelegate() OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event&) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  
  void CreateOptionsMenu();

  
  void GetContentsMask(const gfx::Rect& contents_rect, gfx::Path* path) const;

  
  void GetFrameMask(const gfx::Rect&, gfx::Path* path) const;

  
  void SizeContentsWindow();

  
  
  int GetShelfHeight() const;

  
  int GetBalloonFrameHeight() const;

  int GetTotalWidth() const;
  int GetTotalHeight() const;

  gfx::Rect GetCloseButtonBounds() const;
  gfx::Rect GetOptionsButtonBounds() const;
  gfx::Rect GetLabelBounds() const;

  
  
  gfx::Point GetContentsOffset() const;

  
  gfx::Rect GetContentsRectangle() const;

  
  gfx::Rect GetBoundsForFrameContainer() const;

  
  Balloon* balloon_;

  
  BalloonCollection* collection_;

  
  
  views::Widget* frame_container_;

  
  
  views::Widget* html_container_;

  
  scoped_ptr<BalloonViewHost> html_contents_;

  
  views::ImageButton* close_button_;

  
  views::Label* source_label_;

  
  scoped_ptr<gfx::SlideAnimation> animation_;
  gfx::Rect anim_frame_start_;
  gfx::Rect anim_frame_end_;

  
  scoped_ptr<NotificationOptionsMenuModel> options_menu_model_;
  scoped_ptr<views::MenuRunner> menu_runner_;
  views::MenuButton* options_menu_button_;

  content::NotificationRegistrar notification_registrar_;

  
  bool enable_web_ui_;

  
  bool closed_by_user_;

  
  
  
  bool closed_;

  DISALLOW_COPY_AND_ASSIGN(BalloonViewImpl);
};

#endif  
