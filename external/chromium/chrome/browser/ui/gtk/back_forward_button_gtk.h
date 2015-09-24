// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BACK_FORWARD_BUTTON_GTK_H_
#define CHROME_BROWSER_UI_GTK_BACK_FORWARD_BUTTON_GTK_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "chrome/browser/ui/gtk/custom_button.h"
#include "chrome/browser/ui/gtk/menu_gtk.h"
#include "ui/base/gtk/gtk_signal.h"

class BackForwardMenuModel;
class Browser;

typedef struct _GtkWidget GtkWidget;

class BackForwardButtonGtk : MenuGtk::Delegate {
 public:
  BackForwardButtonGtk(Browser* browser, bool is_forward);
  virtual ~BackForwardButtonGtk();

  
  virtual void StoppedShowing();
  virtual bool AlwaysShowIconForCmd(int command_id) const;

  GtkWidget* widget() { return button_->widget(); }

 private:
  
  CHROMEGTK_CALLBACK_0(BackForwardButtonGtk, void, OnClick);

  
  CHROMEGTK_CALLBACK_1(BackForwardButtonGtk, gboolean, OnButtonPress,
                       GdkEventButton*);

  
  
  
  CHROMEGTK_CALLBACK_1(BackForwardButtonGtk, gboolean, OnMouseMove,
                       GdkEventMotion*);

  
  void ShowBackForwardMenu(int button, guint32 event_time);

  
  scoped_ptr<MenuGtk> menu_;

  scoped_ptr<CustomDrawButton> button_;

  
  Browser* browser_;

  
  bool is_forward_;

  
  scoped_ptr<BackForwardMenuModel> menu_model_;

  
  int y_position_of_last_press_;

  ScopedRunnableMethodFactory<BackForwardButtonGtk> show_menu_factory_;

  DISALLOW_COPY_AND_ASSIGN(BackForwardButtonGtk);
};

#endif  
