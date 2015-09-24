// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_AVATAR_MENU_ITEM_GTK_H_
#define CHROME_BROWSER_UI_GTK_AVATAR_MENU_ITEM_GTK_H_

#include <gtk/gtk.h>

#include "chrome/browser/profiles/avatar_menu.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"

class GtkThemeService;

class AvatarMenuItemGtk : public content::NotificationObserver {
 public:
  
  class Delegate {
   public:
    
    virtual void OpenProfile(size_t profile_index) = 0;

    
    virtual void EditProfile(size_t profile_index) = 0;
  };

  AvatarMenuItemGtk(Delegate* delegate,
                    const AvatarMenu::Item& item,
                    size_t item_index,
                    GtkThemeService* theme_service);
  virtual ~AvatarMenuItemGtk();

  
  GtkWidget* widget() { return widget_.get(); }

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:

  void ShowStatusLabel();
  void ShowEditLink();

  CHROMEGTK_CALLBACK_1(AvatarMenuItemGtk, gboolean, OnProfileClick,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_1(AvatarMenuItemGtk, gboolean, OnProfileEnter,
                       GdkEventCrossing*);
  CHROMEGTK_CALLBACK_1(AvatarMenuItemGtk, gboolean, OnProfileLeave,
                       GdkEventCrossing*);
  CHROMEGTK_CALLBACK_1(AvatarMenuItemGtk, gboolean, OnProfileFocusIn,
                       GdkEventFocus*);
  CHROMEGTK_CALLBACK_1(AvatarMenuItemGtk, gboolean, OnProfileFocusOut,
                       GdkEventFocus*);
  CHROMEGTK_CALLBACK_1(AvatarMenuItemGtk, gboolean, OnProfileKeyPress,
                       GdkEventKey*);
  CHROMEGTK_CALLBACK_1(AvatarMenuItemGtk, gboolean, OnEventBoxExpose,
                       GdkEventExpose*);
  CHROMEGTK_CALLBACK_0(AvatarMenuItemGtk, void, OnEditProfileLinkClicked);

  
  void Init(GtkThemeService* theme_service);

  
  Delegate* delegate_;

  
  AvatarMenu::Item item_;

  
  
  size_t item_index_;

  
  ui::OwnedWidgetGtk widget_;

  
  GtkThemeService* theme_service_;

  
  
  
  GtkWidget* status_label_;

  
  
  GtkWidget* link_alignment_;

  
  
  GtkWidget* edit_profile_link_;

  
  
  GdkColor highlighted_color_;

  
  
  const GdkColor* unhighlighted_color_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(AvatarMenuItemGtk);
};

#endif  
