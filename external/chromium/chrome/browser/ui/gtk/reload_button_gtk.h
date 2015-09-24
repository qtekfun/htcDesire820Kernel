// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_RELOAD_BUTTON_GTK_H_
#define CHROME_BROWSER_UI_GTK_RELOAD_BUTTON_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/timer.h"
#include "chrome/browser/ui/gtk/custom_button.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"

class Browser;
class GtkThemeService;
class LocationBarViewGtk;
class Task;

class ReloadButtonGtk : public NotificationObserver {
 public:
  enum Mode { MODE_RELOAD = 0, MODE_STOP };

  ReloadButtonGtk(LocationBarViewGtk* location_bar, Browser* browser);
  ~ReloadButtonGtk();

  GtkWidget* widget() const { return widget_.get(); }

  
  
  void ChangeMode(Mode mode, bool force);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& );

 private:
  friend class ReloadButtonGtkTest;

  CHROMEGTK_CALLBACK_0(ReloadButtonGtk, void, OnClicked);
  CHROMEGTK_CALLBACK_1(ReloadButtonGtk, gboolean, OnExpose, GdkEventExpose*);
  CHROMEGTK_CALLBACK_1(ReloadButtonGtk,
                       gboolean,
                       OnLeaveNotify,
                       GdkEventCrossing*);
  CHROMEGTK_CALLBACK_4(ReloadButtonGtk,
                       gboolean,
                       OnQueryTooltip,
                       gint,
                       gint,
                       gboolean,
                       GtkTooltip*);

  void UpdateThemeButtons();

  void OnDoubleClickTimer();
  void OnStopToReloadTimer();

  base::OneShotTimer<ReloadButtonGtk> double_click_timer_;
  base::OneShotTimer<ReloadButtonGtk> stop_to_reload_timer_;

  
  LocationBarViewGtk* const location_bar_;
  Browser* const browser_;

  
  Mode intended_mode_;

  
  Mode visible_mode_;

  
  NotificationRegistrar registrar_;

  GtkThemeService* theme_service_;

  CustomDrawButtonBase reload_;
  CustomDrawButtonBase stop_;
  CustomDrawHoverController hover_controller_;

  OwnedWidgetGtk widget_;

  
  
  base::TimeDelta double_click_timer_delay_;
  base::TimeDelta stop_to_reload_timer_delay_;

  
  
  bool testing_mouse_hovered_;
  
  
  int testing_reload_count_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ReloadButtonGtk);
};

#endif  
