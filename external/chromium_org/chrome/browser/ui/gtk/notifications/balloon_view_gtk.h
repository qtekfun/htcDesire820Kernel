// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_GTK_NOTIFICATIONS_BALLOON_VIEW_GTK_H_
#define CHROME_BROWSER_UI_GTK_NOTIFICATIONS_BALLOON_VIEW_GTK_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/notifications/balloon.h"
#include "chrome/browser/ui/gtk/menu_gtk.h"
#include "chrome/browser/ui/gtk/notifications/balloon_view_host_gtk.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

class BalloonCollection;
class CustomDrawButton;
class GtkThemeService;
class MenuGtk;
class NotificationOptionsMenuModel;

namespace gfx {
class SlideAnimation;
}

class BalloonViewImpl : public BalloonView,
                        public MenuGtk::Delegate,
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

  
  virtual void StoppedShowing() OVERRIDE;

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  
  
  
  void DelayedClose(bool by_user);

  
  
  int GetShelfHeight() const;

  
  
  
  int GetDesiredTotalWidth() const;
  int GetDesiredTotalHeight() const;

  
  
  gfx::Point GetContentsOffset() const;

  
  gfx::Rect GetContentsRectangle() const;

  CHROMEGTK_CALLBACK_1(BalloonViewImpl, gboolean, OnContentsExpose,
                       GdkEventExpose*);
  CHROMEGTK_CALLBACK_0(BalloonViewImpl, void, OnCloseButton);
  CHROMEGTK_CALLBACK_1(BalloonViewImpl, gboolean, OnExpose, GdkEventExpose*);
  CHROMEGTK_CALLBACK_1(BalloonViewImpl, void, OnOptionsMenuButton,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_0(BalloonViewImpl, gboolean, OnDestroy);

  
  Balloon* balloon_;

  GtkThemeService* theme_service_;

  
  GtkWidget* frame_container_;

  
  GtkWidget* shelf_;

  
  GtkWidget* hbox_;

  
  GtkWidget* html_container_;

  
  scoped_ptr<BalloonViewHost> html_contents_;

  
  scoped_ptr<CustomDrawButton> close_button_;

  
  scoped_ptr<gfx::SlideAnimation> animation_;
  gfx::Rect anim_frame_start_;
  gfx::Rect anim_frame_end_;

  
  scoped_ptr<MenuGtk> options_menu_;
  scoped_ptr<NotificationOptionsMenuModel> options_menu_model_;
  
  scoped_ptr<CustomDrawButton> options_menu_button_;

  content::NotificationRegistrar notification_registrar_;

  
  bool menu_showing_;

  
  bool pending_close_;

  base::WeakPtrFactory<BalloonViewImpl> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(BalloonViewImpl);
};

#endif  
