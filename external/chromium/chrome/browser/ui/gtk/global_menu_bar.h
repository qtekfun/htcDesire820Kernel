// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GLOBAL_MENU_BAR_H_
#define CHROME_BROWSER_UI_GTK_GLOBAL_MENU_BAR_H_

#include <map>

#include "chrome/browser/command_updater.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"

class Browser;
class BrowserWindowGtk;
struct GlobalMenuBarCommand;

typedef struct _GtkAccelGroup GtkAccelGroup;
typedef struct _GtkWidget GtkWidget;

class GlobalMenuBar : public CommandUpdater::CommandObserver,
                      public NotificationObserver {
 public:
  GlobalMenuBar(Browser* browser, BrowserWindowGtk* window);
  virtual ~GlobalMenuBar();

  GtkWidget* widget() { return menu_bar_; }

 private:
  typedef std::map<int, GtkWidget*> IDMenuItemMap;

  
  void BuildGtkMenuFrom(int menu_str_id,
                        std::map<int, GtkWidget*>* id_to_menu_item,
                        GlobalMenuBarCommand* commands);

  
  virtual void EnabledStateChangedForCommand(int id, bool enabled);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  CHROMEGTK_CALLBACK_0(GlobalMenuBar, void, OnItemActivated);

  Browser* browser_;
  BrowserWindowGtk* browser_window_;

  NotificationRegistrar registrar_;

  
  GtkWidget* menu_bar_;

  
  
  
  GtkAccelGroup* dummy_accel_group_;

  
  
  IDMenuItemMap id_to_menu_item_;

  
  
  bool block_activation_;
};

#endif  
