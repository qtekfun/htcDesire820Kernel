// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_MENU_GTK_H_
#define CHROME_BROWSER_UI_GTK_MENU_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include <string>
#include <vector>

#include "base/task.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/point.h"

class SkBitmap;

namespace ui {
class ButtonMenuItemModel;
class MenuModel;
}

class MenuGtk {
 public:
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    
    
    
    
    virtual void CommandWillBeExecuted() {}

    
    
    
    virtual void StoppedShowing() {}

    
    
    virtual bool AlwaysShowIconForCmd(int command_id) const { return false; }

    
    virtual GtkIconSet* GetIconSetForId(int idr) { return NULL; }

    
    virtual GtkWidget* GetImageForCommandId(int command_id) const;

    static GtkWidget* GetDefaultImageForCommandId(int command_id);
  };

  MenuGtk(MenuGtk::Delegate* delegate, ui::MenuModel* model);
  ~MenuGtk();

  
  void ConnectSignalHandlers();

  
  
  GtkWidget* AppendMenuItemWithLabel(int command_id, const std::string& label);
  GtkWidget* AppendMenuItemWithIcon(int command_id, const std::string& label,
                                    const SkBitmap& icon);
  GtkWidget* AppendCheckMenuItemWithLabel(int command_id,
                                          const std::string& label);
  GtkWidget* AppendSeparator();
  GtkWidget* AppendMenuItem(int command_id, GtkWidget* menu_item);
  GtkWidget* AppendMenuItemToMenu(int index,
                                  ui::MenuModel* model,
                                  GtkWidget* menu_item,
                                  GtkWidget* menu,
                                  bool connect_to_activate);

  
  
  
  
  void PopupForWidget(GtkWidget* widget, int button, guint32 event_time);

  
  
  
  
  void PopupAsContext(const gfx::Point& point, guint32 event_time);

  
  void PopupAsContextForStatusIcon(guint32 event_time, guint32 button,
                                   GtkStatusIcon* icon);

  
  
  void PopupAsFromKeyEvent(GtkWidget* widget);

  
  void Cancel();

  
  
  
  
  
  
  static void WidgetMenuPositionFunc(GtkMenu* menu,
                                     int* x,
                                     int* y,
                                     gboolean* push_in,
                                     void* void_widget);

  
  static void PointMenuPositionFunc(GtkMenu* menu,
                                    int* x,
                                    int* y,
                                    gboolean* push_in,
                                    gpointer userdata);

  GtkWidget* widget() const { return menu_; }

  
  void UpdateMenu();

 private:
  
  GtkWidget* BuildMenuItemWithImage(const std::string& label,
                                    const SkBitmap& icon);

  GtkWidget* BuildMenuItemWithImage(const std::string& label,
                                    GtkWidget* image);

  GtkWidget* BuildMenuItemWithLabel(const std::string& label,
                                    int command_id);

  
  void BuildMenuFromModel();
  
  void BuildSubmenuFromModel(ui::MenuModel* model, GtkWidget* menu);
  
  GtkWidget* BuildButtonMenuItem(ui::ButtonMenuItemModel* model,
                                 GtkWidget* menu);

  void ExecuteCommand(ui::MenuModel* model, int id);

  
  CHROMEGTK_CALLBACK_0(MenuGtk, void, OnMenuItemActivated);

  
  CHROMEGTK_CALLBACK_1(MenuGtk, void, OnMenuButtonPressed, int);

  
  
  CHROMEGTK_CALLBACK_1(MenuGtk, gboolean, OnMenuTryButtonPressed, int);

  
  CHROMEGTK_CALLBACK_0(MenuGtk, void, OnMenuShow);

  
  CHROMEGTK_CALLBACK_0(MenuGtk, void, OnMenuHidden);

  
  static void SetButtonItemInfo(GtkWidget* button, gpointer userdata);

  
  
  static void SetMenuItemInfo(GtkWidget* widget, void* raw_menu);

  
  MenuGtk::Delegate* delegate_;

  
  
  ui::MenuModel* model_;

  
  
  
  GtkAccelGroup* dummy_accel_group_;

  
  
  GtkWidget* menu_;

  
  
  
  static bool block_activation_;

  
  std::vector<MenuGtk*> submenus_we_own_;

  ScopedRunnableMethodFactory<MenuGtk> factory_;
};

#endif  
