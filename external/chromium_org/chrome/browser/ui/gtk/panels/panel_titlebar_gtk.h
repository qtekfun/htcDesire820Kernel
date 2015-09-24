// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_PANELS_PANEL_TITLEBAR_GTK_H_
#define CHROME_BROWSER_UI_GTK_PANELS_PANEL_TITLEBAR_GTK_H_

#include <gtk/gtk.h>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/gtk/titlebar_throb_animation.h"
#include "chrome/browser/ui/panels/panel_constants.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/skia_util.h"

class CustomDrawButton;
class PanelGtk;

namespace content {
class WebContents;
}

class PanelTitlebarGtk {
 public:
  explicit PanelTitlebarGtk(PanelGtk* panel_gtk);
  virtual ~PanelTitlebarGtk();

  void UpdateTextColor();
  void UpdateMinimizeRestoreButtonVisibility();

  
  
  
  
  
  
  
  
  
  
  void SendEnterNotifyToCloseButtonIfUnderMouse();

  void Init();
  void UpdateTitleAndIcon();
  void UpdateThrobber(content::WebContents* web_contents);
  GtkWidget* widget() const;

 private:
  friend class GtkNativePanelTesting;

  void BuildButtons();
  CustomDrawButton* CreateButton(panel::TitlebarButtonType button_type);
  void GetButtonResources(panel::TitlebarButtonType button_type,
                          int* normal_image_id,
                          int* pressed_image_id,
                          int* hover_image_id,
                          int* tooltip_id) const;
  GtkWidget* GetButtonHBox();

  
  CHROMEGTK_CALLBACK_0(PanelTitlebarGtk, void, OnButtonClicked);

  CustomDrawButton* close_button() const { return close_button_.get(); }
  CustomDrawButton* minimize_button() const { return minimize_button_.get(); }
  CustomDrawButton* restore_button() const { return restore_button_.get(); }

  SkColor GetTextColor() const;

  
  PanelGtk* panel_gtk_;

  
  GtkWidget* container_;

  
  GtkWidget* titlebar_right_buttons_vbox_;

  
  GtkWidget* titlebar_right_buttons_hbox_;

  
  GtkWidget* icon_;
  GtkWidget* title_;

  
  scoped_ptr<CustomDrawButton> close_button_;
  scoped_ptr<CustomDrawButton> minimize_button_;
  scoped_ptr<CustomDrawButton> restore_button_;

  TitlebarThrobAnimation throbber_;

  DISALLOW_COPY_AND_ASSIGN(PanelTitlebarGtk);
};

#endif  
