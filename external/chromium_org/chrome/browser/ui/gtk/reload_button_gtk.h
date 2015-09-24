// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_RELOAD_BUTTON_GTK_H_
#define CHROME_BROWSER_UI_GTK_RELOAD_BUTTON_GTK_H_

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/timer/timer.h"
#include "chrome/browser/ui/gtk/custom_button.h"
#include "chrome/browser/ui/gtk/menu_gtk.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/base/models/simple_menu_model.h"

class Browser;
class GtkThemeService;
class LocationBarViewGtk;

class ReloadButtonGtk : public content::NotificationObserver,
                        MenuGtk::Delegate,
                        public ui::SimpleMenuModel::Delegate {
 public:
  enum Mode { MODE_RELOAD = 0, MODE_STOP };

  ReloadButtonGtk(LocationBarViewGtk* location_bar, Browser* browser);
  virtual ~ReloadButtonGtk();

  GtkWidget* widget() const { return widget_.get(); }

  
  
  void ChangeMode(Mode mode, bool force);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void StoppedShowing() OVERRIDE;

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool IsCommandIdVisible(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
     int command_id,
     ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

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

  
  CHROMEGTK_CALLBACK_1(ReloadButtonGtk,
                       gboolean,
                       OnButtonPress,
                       GdkEventButton*);

  
  
  
  CHROMEGTK_CALLBACK_1(ReloadButtonGtk,
                       gboolean,
                       OnMouseMove,
                       GdkEventMotion*);

  void UpdateThemeButtons();

  void OnDoubleClickTimer();
  void OnStopToReloadTimer();

  
  void ShowReloadMenu(int button, guint32 event_time);

  
  void DoReload(int command);

  
  bool ReloadMenuEnabled();
  void ClearCache();

  base::OneShotTimer<ReloadButtonGtk> double_click_timer_;
  base::OneShotTimer<ReloadButtonGtk> stop_to_reload_timer_;

  
  LocationBarViewGtk* const location_bar_;
  Browser* const browser_;

  
  Mode intended_mode_;

  
  Mode visible_mode_;

  
  content::NotificationRegistrar registrar_;

  GtkThemeService* theme_service_;

  CustomDrawButtonBase reload_;
  CustomDrawButtonBase stop_;
  CustomDrawHoverController hover_controller_;

  ui::OwnedWidgetGtk widget_;

  
  
  base::TimeDelta double_click_timer_delay_;
  base::TimeDelta stop_to_reload_timer_delay_;

  
  int y_position_of_last_press_;
  
  scoped_ptr<MenuGtk> menu_;
  
  scoped_ptr<ui::SimpleMenuModel> menu_model_;
  
  bool menu_visible_;

  
  
  bool testing_mouse_hovered_;
  
  
  int testing_reload_count_;

  base::WeakPtrFactory<ReloadButtonGtk> weak_factory_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ReloadButtonGtk);
};

#endif  
