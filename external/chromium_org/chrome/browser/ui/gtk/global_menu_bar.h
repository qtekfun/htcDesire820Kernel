// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GLOBAL_MENU_BAR_H_
#define CHROME_BROWSER_UI_GTK_GLOBAL_MENU_BAR_H_

#include <map>

#include "base/compiler_specific.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/command_observer.h"
#include "chrome/browser/ui/gtk/global_history_menu.h"
#include "ui/base/gtk/gtk_signal.h"

class Browser;
struct GlobalMenuBarCommand;
class GlobalMenuOwner;

typedef struct _GtkAccelGroup GtkAccelGroup;
typedef struct _GtkWidget GtkWidget;

class GlobalMenuBar : public CommandObserver {
 public:
  static const int TAG_NORMAL = 0;
  static const int TAG_MOST_VISITED = 1;
  static const int TAG_RECENTLY_CLOSED = 2;
  static const int TAG_MOST_VISITED_HEADER = 3;
  static const int TAG_RECENTLY_CLOSED_HEADER = 4;
  static const int TAG_BOOKMARK_CLEARABLE = 5;

  explicit GlobalMenuBar(Browser* browser);
  virtual ~GlobalMenuBar();

  
  
  virtual void Disable();

  GtkWidget* widget() { return menu_bar_; }

 private:
  typedef std::map<int, GtkWidget*> CommandIDMenuItemMap;

  
  void BuildGtkMenuFrom(int menu_str_id,
                        std::map<int, GtkWidget*>* id_to_menu_item,
                        GlobalMenuBarCommand* commands,
                        GlobalMenuOwner* owner);

  
  GtkWidget* BuildMenuItem(int string_id,
                           int command_id,
                           int tag_id,
                           std::map<int, GtkWidget*>* id_to_menu_item,
                           GtkWidget* menu_to_add_to);

  
  virtual void EnabledStateChangedForCommand(int id, bool enabled) OVERRIDE;

  
  void OnBookmarkBarVisibilityChanged();

  CHROMEGTK_CALLBACK_0(GlobalMenuBar, void, OnItemActivated);

  Browser* browser_;

  
  PrefChangeRegistrar pref_change_registrar_;

  
  GtkWidget* menu_bar_;

  
  
  GlobalHistoryMenu history_menu_;

  
  
  
  GtkAccelGroup* dummy_accel_group_;

  
  
  CommandIDMenuItemMap id_to_menu_item_;

  
  
  bool block_activation_;
};

#endif  
