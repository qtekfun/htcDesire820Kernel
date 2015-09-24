// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_CUSTOM_BUTTON_H_
#define CHROME_BROWSER_UI_GTK_CUSTOM_BUTTON_H_
#pragma once

#include <gtk/gtk.h>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/slide_animation.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/rect.h"

class CairoCachedSurface;
class GtkThemeService;
class SkBitmap;


class CustomDrawButtonBase : public NotificationObserver {
 public:
  
  
  CustomDrawButtonBase(GtkThemeService* theme_provider,
                       int normal_id,
                       int pressed_id,
                       int hover_id,
                       int disabled_id);

  ~CustomDrawButtonBase();

  
  
  void set_flipped(bool flipped) { flipped_ = flipped; }

  
  int Width() const;
  int Height() const;

  gboolean OnExpose(GtkWidget* widget, GdkEventExpose* e, gdouble hover_state);

  void set_paint_override(int state) { paint_override_ = state; }
  int paint_override() const { return paint_override_; }

  
  void SetBackground(SkColor color, SkBitmap* image, SkBitmap* mask);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  CairoCachedSurface* PixbufForState(int state);

  
  
  scoped_ptr<CairoCachedSurface> surfaces_[GTK_STATE_INSENSITIVE + 1];

  
  scoped_ptr<CairoCachedSurface> background_image_;

  
  int paint_override_;

  
  
  int normal_id_;
  int pressed_id_;
  int hover_id_;
  int disabled_id_;
  GtkThemeService* theme_service_;

  
  
  
  bool flipped_;

  
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(CustomDrawButtonBase);
};

class CustomDrawHoverController : public ui::AnimationDelegate {
 public:
  explicit CustomDrawHoverController(GtkWidget* widget);
  CustomDrawHoverController();

  virtual ~CustomDrawHoverController();

  void Init(GtkWidget* widget);

  double GetCurrentValue() {
    return slide_animation_.GetCurrentValue();
  }

 private:
  virtual void AnimationProgressed(const ui::Animation* animation);

  CHROMEGTK_CALLBACK_1(CustomDrawHoverController, gboolean, OnEnter,
                       GdkEventCrossing*);
  CHROMEGTK_CALLBACK_1(CustomDrawHoverController, gboolean, OnLeave,
                       GdkEventCrossing*);

  ui::SlideAnimation slide_animation_;
  GtkWidget* widget_;
};

class CustomDrawButton : public NotificationObserver {
 public:
  
  
  CustomDrawButton(int normal_id,
                   int pressed_id,
                   int hover_id,
                   int disabled_id);

  
  
  CustomDrawButton(GtkThemeService* theme_provider,
                   int normal_id,
                   int pressed_id,
                   int hover_id,
                   int disabled_id,
                   const char* stock_id,
                   GtkIconSize stock_size);

  
  
  CustomDrawButton(GtkThemeService* theme_provider,
                   int normal_id,
                   int pressed_id,
                   int hover_id,
                   int disabled_id,
                   GtkWidget* native_widget);

  ~CustomDrawButton();

  void Init();

  
  
  void set_flipped(bool flipped) { button_base_.set_flipped(flipped); }

  GtkWidget* widget() const { return widget_.get(); }

  gfx::Rect bounds() const {
      return gfx::Rect(widget_->allocation.x,
                       widget_->allocation.y,
                       widget_->allocation.width,
                       widget_->allocation.height);
  }

  int width() const { return widget_->allocation.width; }
  int height() const { return widget_->allocation.height; }

  
  
  void SetPaintOverride(GtkStateType state);

  
  void UnsetPaintOverride();

  
  void SetBackground(SkColor color, SkBitmap* image, SkBitmap* mask);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  static CustomDrawButton* CloseButton(GtkThemeService* theme_provider);

 private:
  
  void SetBrowserTheme();

  
  
  bool UseGtkTheme();

  
  CHROMEGTK_CALLBACK_1(CustomDrawButton, gboolean, OnCustomExpose,
                       GdkEventExpose*);

  
  OwnedWidgetGtk widget_;

  CustomDrawButtonBase button_base_;

  CustomDrawHoverController hover_controller_;

  
  OwnedWidgetGtk native_widget_;

  
  GtkThemeService* theme_service_;

  
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(CustomDrawButton);
};

#endif  
