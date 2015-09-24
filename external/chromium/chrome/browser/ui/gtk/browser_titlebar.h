// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BROWSER_TITLEBAR_H_
#define CHROME_BROWSER_UI_GTK_BROWSER_TITLEBAR_H_
#pragma once

#include <gtk/gtk.h>

#include "base/memory/scoped_ptr.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/base/x/active_window_watcher_x.h"

class BrowserWindowGtk;
class CustomDrawButton;
class GtkThemeService;
class MenuGtk;
class PopupPageMenuModel;
class TabContents;

class BrowserTitlebar : public NotificationObserver,
                        public ui::ActiveWindowWatcherX::Observer,
                        public ui::SimpleMenuModel::Delegate {
 public:
  
  
  static const char kDefaultButtonString[];

  BrowserTitlebar(BrowserWindowGtk* browser_window, GtkWindow* window);
  virtual ~BrowserTitlebar();

  GtkWidget* widget() {
    return container_;
  }

  void set_window(GtkWindow* window) { window_ = window; }

  
  void BuildButtons(const std::string& button_string);

  
  
  
  void UpdateCustomFrame(bool use_custom_frame);

  
  void UpdateTitleAndIcon();

  
  
  
  void UpdateThrobber(TabContents* tab_contents);

  
  
  
  void ShowContextMenu(GdkEventButton* event);

 private:
  
  
  class Throbber {
   public:
    Throbber() : current_frame_(0), current_waiting_frame_(0) {}

    
    
    
    GdkPixbuf* GetNextFrame(bool is_waiting);

    
    void Reset();
   private:
    
    static void InitFrames();

    int current_frame_;
    int current_waiting_frame_;
  };

  class ContextMenuModel : public ui::SimpleMenuModel {
   public:
    explicit ContextMenuModel(ui::SimpleMenuModel::Delegate* delegate);
  };

  
  
  
  void Init();

  
  
  
  
  GtkWidget* GetButtonHBox(bool left_side);

  
  
  CustomDrawButton* BuildTitlebarButton(int image, int image_pressed,
                                        int image_hot, GtkWidget* box,
                                        int tooltip);

  
  void UpdateTitlebarAlignment();

  
  
  void UpdateTextColor();

  
  void ShowFaviconMenu(GdkEventButton* event);

  
  
  void MaximizeButtonClicked();

  
  
  void UpdateMaximizeRestoreVisibility();

  
  
  CHROMEG_CALLBACK_1(BrowserTitlebar, gboolean, OnWindowStateChanged,
                     GtkWindow*, GdkEventWindowState*);

  
  CHROMEGTK_CALLBACK_1(BrowserTitlebar, gboolean, OnScroll,
                       GdkEventScroll*);

  
  CHROMEGTK_CALLBACK_0(BrowserTitlebar, void, OnButtonClicked);

  
  CHROMEGTK_CALLBACK_1(BrowserTitlebar, gboolean, OnButtonPressed,
                       GdkEventButton*);

  

  
  virtual bool IsCommandIdEnabled(int command_id) const;
  virtual bool IsCommandIdChecked(int command_id) const;
  virtual void ExecuteCommand(int command_id);
  virtual bool GetAcceleratorForCommandId(int command_id,
                                          ui::Accelerator* accelerator);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void ActiveWindowChanged(GdkWindow* active_window);

  
  BrowserWindowGtk* browser_window_;
  GtkWindow* window_;

  
  GtkWidget* container_;

  
  GtkWidget* container_hbox_;

  
  
  
  
  GtkWidget* titlebar_left_buttons_vbox_;
  GtkWidget* titlebar_right_buttons_vbox_;

  
  GtkWidget* titlebar_left_buttons_hbox_;
  GtkWidget* titlebar_right_buttons_hbox_;

  
  
  
  GtkWidget* titlebar_left_spy_frame_;
  GtkWidget* titlebar_right_spy_frame_;

  
  
  GtkWidget* top_padding_left_;
  GtkWidget* top_padding_right_;

  
  
  GtkWidget* titlebar_alignment_;

  
  GtkWidget* app_mode_favicon_;
  GtkWidget* app_mode_title_;

  
  bool using_custom_frame_;

  
  
  bool window_has_focus_;

  
  
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

  
  Throbber throbber_;

  
  GtkThemeService* theme_service_;

  NotificationRegistrar registrar_;
};

#endif  
