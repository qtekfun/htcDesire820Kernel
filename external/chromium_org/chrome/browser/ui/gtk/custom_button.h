// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_CUSTOM_BUTTON_H_
#define CHROME_BROWSER_UI_GTK_CUSTOM_BUTTON_H_

#include <gtk/gtk.h>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"
#include "ui/gfx/rect.h"

class GtkThemeService;
class SkBitmap;

namespace gfx {
class CairoCachedSurface;
}


class CustomDrawButtonBase : public content::NotificationObserver {
 public:
  
  
  CustomDrawButtonBase(GtkThemeService* theme_provider,
                       int normal_id,
                       int pressed_id,
                       int hover_id,
                       int disabled_id);

  virtual ~CustomDrawButtonBase();

  
  
  void set_flipped(bool flipped) { flipped_ = flipped; }

  
  int Width() const;
  int Height() const;

  gboolean OnExpose(GtkWidget* widget, GdkEventExpose* e, gdouble hover_state);

  void set_paint_override(int state) { paint_override_ = state; }
  int paint_override() const { return paint_override_; }

  
  void SetBackground(SkColor color,
                     const SkBitmap& image,
                     const SkBitmap& mask);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  
  gfx::CairoCachedSurface* PixbufForState(int state);

  
  
  scoped_ptr<gfx::CairoCachedSurface> surfaces_[GTK_STATE_INSENSITIVE + 1];

  
  scoped_ptr<gfx::CairoCachedSurface> background_image_;

  
  int paint_override_;

  
  
  int normal_id_;
  int pressed_id_;
  int hover_id_;
  int disabled_id_;
  GtkThemeService* theme_service_;

  
  
  
  bool flipped_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(CustomDrawButtonBase);
};

class CustomDrawHoverController : public gfx::AnimationDelegate {
 public:
  explicit CustomDrawHoverController(GtkWidget* widget);
  CustomDrawHoverController();

  virtual ~CustomDrawHoverController();

  void Init(GtkWidget* widget);

  double GetCurrentValue() {
    return slide_animation_.GetCurrentValue();
  }

 private:
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  CHROMEGTK_CALLBACK_1(CustomDrawHoverController, gboolean, OnEnter,
                       GdkEventCrossing*);
  CHROMEGTK_CALLBACK_1(CustomDrawHoverController, gboolean, OnLeave,
                       GdkEventCrossing*);

  gfx::SlideAnimation slide_animation_;
  GtkWidget* widget_;
};

class CustomDrawButton : public content::NotificationObserver {
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

  virtual ~CustomDrawButton();

  void Init();

  
  
  void ForceChromeTheme();

  
  
  void set_flipped(bool flipped) { button_base_.set_flipped(flipped); }

  GtkWidget* widget() const { return widget_.get(); }

  
  GtkAllocation WidgetAllocation() const;

  
  int SurfaceWidth() const;
  int SurfaceHeight() const;

  
  
  void SetPaintOverride(GtkStateType state);

  
  void UnsetPaintOverride();

  
  void SetBackground(SkColor color,
                     const SkBitmap& image,
                     const SkBitmap& mask);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  static CustomDrawButton* CloseButtonBar(GtkThemeService* theme_provider);

  
  
  static CustomDrawButton* CloseButtonBubble(GtkThemeService* theme_provider);

 private:
  
  void SetBrowserTheme();

  
  
  bool UseGtkTheme();

  
  CHROMEGTK_CALLBACK_1(CustomDrawButton, gboolean, OnCustomExpose,
                       GdkEventExpose*);

  
  ui::OwnedWidgetGtk widget_;

  CustomDrawButtonBase button_base_;

  CustomDrawHoverController hover_controller_;

  
  ui::OwnedWidgetGtk native_widget_;

  
  GtkThemeService* theme_service_;

  
  bool forcing_chrome_theme_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(CustomDrawButton);
};

#endif  
