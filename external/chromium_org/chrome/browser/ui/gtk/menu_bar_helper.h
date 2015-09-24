// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_MENU_BAR_HELPER_H_
#define CHROME_BROWSER_UI_GTK_MENU_BAR_HELPER_H_

#include <gtk/gtk.h>

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "ui/base/gtk/gtk_signal.h"

namespace ui {
class GtkSignalRegistrar;
}

class MenuBarHelper {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void PopupForButton(GtkWidget* button) = 0;
    virtual void PopupForButtonNextTo(GtkWidget* button,
                                      GtkMenuDirectionType dir) = 0;
  };

  
  explicit MenuBarHelper(Delegate* delegate);
  ~MenuBarHelper();

  
  
  void MenuStartedShowing(GtkWidget* button, GtkWidget* menu);

  
  void Add(GtkWidget* button);

  
  void Remove(GtkWidget* button);

  
  void Clear();

 private:
  CHROMEGTK_CALLBACK_0(MenuBarHelper, void, OnMenuHiddenOrDestroyed);
  CHROMEGTK_CALLBACK_1(MenuBarHelper, gboolean, OnMenuMotionNotify,
                       GdkEventMotion*);
  CHROMEGTK_CALLBACK_1(MenuBarHelper, void, OnMenuMoveCurrent,
                       GtkMenuDirectionType);

  
  
  std::vector<GtkWidget*> buttons_;

  
  GtkWidget* button_showing_menu_;

  
  GtkWidget* showing_menu_;

  
  
  std::vector<GtkWidget*> submenus_;

  
  
  scoped_ptr<ui::GtkSignalRegistrar> signal_handlers_;

  Delegate* delegate_;
};

#endif  
