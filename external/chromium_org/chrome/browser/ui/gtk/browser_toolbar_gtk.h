// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BROWSER_TOOLBAR_GTK_H_
#define CHROME_BROWSER_UI_GTK_BROWSER_TOOLBAR_GTK_H_

#include <gtk/gtk.h>
#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/command_observer.h"
#include "chrome/browser/ui/gtk/custom_button.h"
#include "chrome/browser/ui/gtk/menu_gtk.h"
#include "chrome/browser/ui/toolbar/wrench_menu_model.h"
#include "content/public/browser/host_zoom_map.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/gtk_signal_registrar.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/base/models/simple_menu_model.h"

class BackForwardButtonGtk;
class Browser;
class BrowserActionsToolbarGtk;
class BrowserWindowGtk;
class CustomDrawButton;
class GtkThemeService;
class LocationBar;
class LocationBarViewGtk;
class ReloadButtonGtk;

namespace content {
class WebContents;
}

class BrowserToolbarGtk : public CommandObserver,
                          public ui::AcceleratorProvider,
                          public MenuGtk::Delegate,
                          public content::NotificationObserver {
 public:
  BrowserToolbarGtk(Browser* browser, BrowserWindowGtk* window);
  virtual ~BrowserToolbarGtk();

  
  
  void Init(GtkWindow* top_level_window);

  
  void SetViewIDs();

  void Show();
  void Hide();

  
  GtkWidget* widget() {
    return event_box_;
  }

  
  Browser* browser() {
    return browser_;
  }

  virtual LocationBar* GetLocationBar() const;

  ReloadButtonGtk* GetReloadButton() { return reload_.get(); }

  GtkWidget* GetAppMenuButton() { return wrench_menu_button_->widget(); }

  BrowserActionsToolbarGtk* GetBrowserActionsToolbar() {
    return actions_toolbar_.get();
  }

  LocationBarViewGtk* GetLocationBarView() { return location_bar_.get(); }

  
  
  void UpdateForBookmarkBarVisibility(bool show_bottom_padding);

  void ShowAppMenu();

  
  virtual void EnabledStateChangedForCommand(int id, bool enabled) OVERRIDE;

  
  virtual void StoppedShowing() OVERRIDE;
  virtual GtkIconSet* GetIconSetForId(int idr) OVERRIDE;
  virtual bool AlwaysShowIconForCmd(int command_id) const OVERRIDE;

  
  virtual bool GetAcceleratorForCommandId(
      int id,
      ui::Accelerator* accelerator) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  bool IsWrenchMenuShowing() const;

  
  void UpdateWebContents(content::WebContents* contents);

 private:
  void OnZoomLevelChanged(const content::HostZoomMap::ZoomLevelChange& host);

  
  void SetUpDragForHomeButton();

  
  
  
  bool UpdateRoundedness();

  
  
  CHROMEGTK_CALLBACK_1(BrowserToolbarGtk, gboolean, OnAlignmentExpose,
                       GdkEventExpose*);
  CHROMEGTK_CALLBACK_1(BrowserToolbarGtk, gboolean, OnLocationHboxExpose,
                       GdkEventExpose*);

  
  CHROMEGTK_CALLBACK_0(BrowserToolbarGtk, void, OnButtonClick);

  
  CHROMEGTK_CALLBACK_1(BrowserToolbarGtk, gboolean, OnMenuButtonPressEvent,
                       GdkEventButton*);

  
  CHROMEGTK_CALLBACK_6(BrowserToolbarGtk, void, OnDragDataReceived,
                       GdkDragContext*, gint, gint, GtkSelectionData*,
                       guint, guint);

  
  CHROMEGTK_CALLBACK_1(BrowserToolbarGtk, gboolean, OnWrenchMenuButtonExpose,
                       GdkEventExpose*);

  static void SetSyncMenuLabel(GtkWidget* widget, gpointer userdata);

  
  
  bool ShouldOnlyShowLocation() const;

  
  void RebuildWrenchMenu();

  void UpdateShowHomeButton();

  
  
  
  
  GtkWidget* event_box_;

  
  GtkWidget* alignment_;

  
  
  GtkWidget* toolbar_;

  
  
  
  
  GtkWidget* toolbar_left_;

  
  GtkWidget* location_hbox_;

  
  scoped_ptr<LocationBarViewGtk> location_bar_;

  
  scoped_ptr<BackForwardButtonGtk> back_, forward_;
  scoped_ptr<CustomDrawButton> home_;
  scoped_ptr<ReloadButtonGtk> reload_;
  scoped_ptr<BrowserActionsToolbarGtk> actions_toolbar_;
  scoped_ptr<CustomDrawButton> wrench_menu_button_;

  
  GtkWidget* wrench_menu_image_;

  GtkThemeService* theme_service_;

  scoped_ptr<MenuGtk> wrench_menu_;

  scoped_ptr<WrenchMenuModel> wrench_menu_model_;

  
  bool is_wrench_menu_model_valid_;

  Browser* browser_;
  BrowserWindowGtk* window_;

  
  BooleanPrefMember show_home_button_;

  
  StringPrefMember home_page_;
  BooleanPrefMember home_page_is_new_tab_page_;

  scoped_ptr<content::HostZoomMap::Subscription> zoom_subscription_;
  content::NotificationRegistrar registrar_;

  
  
  ui::OwnedWidgetGtk offscreen_entry_;

  
  scoped_ptr<ui::GtkSignalRegistrar> drop_handler_;

  DISALLOW_COPY_AND_ASSIGN(BrowserToolbarGtk);
};

#endif  
