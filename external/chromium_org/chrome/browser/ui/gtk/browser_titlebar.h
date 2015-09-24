// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BROWSER_TITLEBAR_H_
#define CHROME_BROWSER_UI_GTK_BROWSER_TITLEBAR_H_

#include <gtk/gtk.h>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/ui/gtk/titlebar_throb_animation.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/base/x/active_window_watcher_x_observer.h"

class AvatarMenuButtonGtk;
class BrowserWindowGtk;
class CustomDrawButton;
class GtkThemeService;
class MenuGtk;
class PopupPageMenuModel;

namespace content {
class WebContents;
}

class BrowserTitlebar : public content::NotificationObserver,
                        public ui::ActiveWindowWatcherXObserver,
                        public ui::SimpleMenuModel::Delegate {
 public:
  
  
  static const char kDefaultButtonString[];

  BrowserTitlebar(BrowserWindowGtk* browser_window, GtkWindow* window);
  virtual ~BrowserTitlebar();

  
  void UpdateTitleAndIcon();

  GtkWidget* widget() {
    return container_;
  }

  void set_window(GtkWindow* window) { window_ = window; }

  
  
  
  void Init();

  
  void BuildButtons(const std::string& button_string);

  
  
  
  void UpdateCustomFrame(bool use_custom_frame);

  
  
  
  void UpdateThrobber(content::WebContents* web_contents);

  
  
  
  void ShowContextMenu(GdkEventButton* event);

  AvatarMenuButtonGtk* avatar_button() { return avatar_button_.get(); }

 private:
  class ContextMenuModel : public ui::SimpleMenuModel {
   public:
    explicit ContextMenuModel(ui::SimpleMenuModel::Delegate* delegate);
  };

  
  
  bool BuildButton(const std::string& button_token, bool left_side);

  
  
  void GetButtonResources(const std::string& button_name,
                          int* normal_image_id,
                          int* pressed_image_id,
                          int* hover_image_id,
                          int* tooltip_id) const;

  
  
  CustomDrawButton* CreateTitlebarButton(const std::string& button_name,
                                         bool left_side);

  
  
  
  
  GtkWidget* GetButtonHBox(bool left_side);

  
  void UpdateButtonBackground(CustomDrawButton* button);

  
  
  void UpdateTextColor();

  
  void UpdateAvatarLabel();

  
  void UpdateTitlebarAlignment();

  
  
  void UpdateAvatar();

  
  
  void MaximizeButtonClicked();

  
  
  void UpdateMaximizeRestoreVisibility();

  
  
  CHROMEG_CALLBACK_1(BrowserTitlebar, gboolean, OnWindowStateChanged,
                     GtkWindow*, GdkEventWindowState*);

  
  CHROMEGTK_CALLBACK_1(BrowserTitlebar, gboolean, OnScroll,
                       GdkEventScroll*);

  
  CHROMEGTK_CALLBACK_0(BrowserTitlebar, void, OnButtonClicked);

  
  CHROMEGTK_CALLBACK_1(BrowserTitlebar, gboolean,
                       OnFaviconMenuButtonPressed, GdkEventButton*);

  
  CHROMEGTK_CALLBACK_1(BrowserTitlebar, gboolean,
                       OnAvatarLabelButtonPressed, GdkEventButton*);

  

  
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void ActiveWindowChanged(GdkWindow* active_window) OVERRIDE;

  
  bool ShouldDisplayAvatar();

  
  
  bool IsOffTheRecord();

  
  BrowserWindowGtk* browser_window_;
  GtkWindow* window_;

  
  GtkWidget* container_;

  
  GtkWidget* container_hbox_;

  
  
  
  
  GtkWidget* titlebar_left_buttons_vbox_;
  GtkWidget* titlebar_right_buttons_vbox_;

  
  GtkWidget* titlebar_left_buttons_hbox_;
  GtkWidget* titlebar_right_buttons_hbox_;

  
  
  
  
  GtkWidget* titlebar_left_avatar_frame_;
  GtkWidget* titlebar_right_avatar_frame_;

  
  
  
  
  GtkWidget* titlebar_left_label_frame_;
  GtkWidget* titlebar_right_label_frame_;

  
  GtkWidget* avatar_;

  
  
  GtkWidget* avatar_label_;
  GtkWidget* avatar_label_bg_;

  
  
  GtkWidget* top_padding_left_;
  GtkWidget* top_padding_right_;

  
  
  GtkWidget* titlebar_alignment_;

  
  GtkWidget* app_mode_favicon_;
  GtkWidget* app_mode_title_;

  
  bool using_custom_frame_;

  
  
  bool window_has_focus_;

  
  bool display_avatar_on_left_;

  
  
  GtkRequisition close_button_req_;
  GtkRequisition minimize_button_req_;
  GtkRequisition restore_button_req_;

  
  scoped_ptr<CustomDrawButton> minimize_button_;
  scoped_ptr<CustomDrawButton> maximize_button_;
  scoped_ptr<CustomDrawButton> restore_button_;
  scoped_ptr<CustomDrawButton> close_button_;

  
  scoped_ptr<MenuGtk> context_menu_;
  scoped_ptr<ContextMenuModel> context_menu_model_;

  
  scoped_ptr<MenuGtk> favicon_menu_;
  scoped_ptr<PopupPageMenuModel> favicon_menu_model_;

  
  TitlebarThrobAnimation throbber_;

  
  scoped_ptr<AvatarMenuButtonGtk> avatar_button_;

  
  GtkThemeService* theme_service_;

  content::NotificationRegistrar registrar_;
};

#endif  
