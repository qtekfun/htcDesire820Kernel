// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_AVATAR_MENU_BUTTON_GTK_H_
#define CHROME_BROWSER_UI_GTK_AVATAR_MENU_BUTTON_GTK_H_

#include <gtk/gtk.h>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/gtk/bubble/bubble_gtk.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"

class Browser;
class SkBitmap;

namespace gfx {
class Image;
}

class AvatarMenuButtonGtk {
 public:
  explicit AvatarMenuButtonGtk(Browser* browser);

  ~AvatarMenuButtonGtk();

  
  GtkWidget* widget() const { return widget_.get(); }

  
  void set_menu_frame_style(BubbleGtk::FrameStyle frame_style) {
    frame_style_ = frame_style;
  }

  
  void SetIcon(const gfx::Image& icon, bool is_gaia_picture);

  
  void ShowAvatarBubble();

 private:
  CHROMEGTK_CALLBACK_1(AvatarMenuButtonGtk, gboolean, OnButtonPressed,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_1(AvatarMenuButtonGtk, void, OnSizeAllocate,
                       GtkAllocation*);

  void UpdateButtonIcon();

  
  ui::OwnedWidgetGtk widget_;

  
  GtkWidget* image_;

  
  Browser* browser_;

  
  BubbleGtk::FrameStyle frame_style_;

  scoped_ptr<gfx::Image> icon_;
  bool is_gaia_picture_;
  int old_height_;

  DISALLOW_COPY_AND_ASSIGN(AvatarMenuButtonGtk);
};

#endif  
