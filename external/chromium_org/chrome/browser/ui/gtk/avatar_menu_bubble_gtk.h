// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_AVATAR_MENU_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_AVATAR_MENU_BUBBLE_GTK_H_

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "chrome/browser/profiles/avatar_menu_observer.h"
#include "chrome/browser/ui/gtk/avatar_menu_item_gtk.h"
#include "chrome/browser/ui/gtk/bubble/bubble_gtk.h"
#include "ui/base/gtk/gtk_signal.h"

class AvatarMenu;
class Browser;
class GtkThemeService;

class AvatarMenuBubbleGtk : public BubbleDelegateGtk,
                            public AvatarMenuObserver,
                            public AvatarMenuItemGtk::Delegate {
 public:
  AvatarMenuBubbleGtk(Browser* browser,
                      GtkWidget* anchor,
                      BubbleGtk::FrameStyle arrow,
                      const gfx::Rect* rect);
  virtual ~AvatarMenuBubbleGtk();

  
  virtual void BubbleClosing(BubbleGtk* bubble,
                             bool closed_by_escape) OVERRIDE;

  
  virtual void OnAvatarMenuChanged(
      AvatarMenu* avatar_menu) OVERRIDE;

  
  virtual void OpenProfile(size_t profile_index) OVERRIDE;
  virtual void EditProfile(size_t profile_index) OVERRIDE;

 private:
  
  CHROMEGTK_CALLBACK_0(AvatarMenuBubbleGtk, void, OnDestroy);
  CHROMEGTK_CALLBACK_1(AvatarMenuBubbleGtk, void, OnSizeRequest,
                       GtkRequisition*);
  CHROMEGTK_CALLBACK_0(AvatarMenuBubbleGtk, void, OnNewProfileLinkClicked);
  CHROMEGTK_CALLBACK_0(AvatarMenuBubbleGtk, void, OnSwitchProfileLinkClicked);

  
  void InitContents();

  
  void InitMenuContents();

  
  void InitManagedUserContents();

  
  void CloseBubble();

  
  scoped_ptr<AvatarMenu> avatar_menu_;

  
  GtkWidget* contents_;

  
  
  GtkWidget* inner_contents_;

  
  BubbleGtk* bubble_;

  
  GtkThemeService* theme_service_;

  
  
  GtkWidget* new_profile_link_;

  
  ScopedVector<AvatarMenuItemGtk> items_;

  
  
  int minimum_width_;

  
  bool switching_;

  DISALLOW_COPY_AND_ASSIGN(AvatarMenuBubbleGtk);
};

#endif  
