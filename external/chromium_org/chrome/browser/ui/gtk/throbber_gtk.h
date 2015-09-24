// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_THROBBER_GTK_H_
#define CHROME_BROWSER_UI_GTK_THROBBER_GTK_H_

#include <gtk/gtk.h>

#include "base/compiler_specific.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"
#include "ui/gfx/image/image.h"

class GtkThemeService;
class SkBitmap;

class ThrobberGtk : public gfx::AnimationDelegate,
                    public content::NotificationObserver {
 public:
  
  explicit ThrobberGtk(GtkThemeService* theme_service);
  virtual ~ThrobberGtk();

  
  void Start();
  void Stop();

  GtkWidget* widget() const { return widget_.get(); }

  
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  CHROMEGTK_CALLBACK_1(ThrobberGtk, gboolean, OnExpose, GdkEventExpose*);

  
  void Init();

  
  void LoadFrames();

  content::NotificationRegistrar registrar_;

  
  GtkThemeService* theme_service_;

  
  ui::OwnedWidgetGtk widget_;

  
  gfx::SlideAnimation animation_;

  
  gfx::Image frames_;

  
  int num_frames_;

  DISALLOW_COPY_AND_ASSIGN(ThrobberGtk);
};

#endif  
