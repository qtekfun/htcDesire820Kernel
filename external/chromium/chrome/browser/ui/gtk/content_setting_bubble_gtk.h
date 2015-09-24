// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_CONTENT_SETTING_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_CONTENT_SETTING_BUBBLE_GTK_H_
#pragma once

#include <map>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/gtk/info_bubble_gtk.h"
#include "chrome/common/content_settings_types.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"

class ContentSettingBubbleModel;
class Profile;
class TabContents;

class ContentSettingBubbleGtk : public InfoBubbleGtkDelegate,
                                public NotificationObserver {
 public:
   ContentSettingBubbleGtk(
       GtkWidget* anchor,
       InfoBubbleGtkDelegate* delegate,
       ContentSettingBubbleModel* content_setting_bubble_model,
       Profile* profile, TabContents* tab_contents);
  virtual ~ContentSettingBubbleGtk();

  
  void Close();

 private:
  typedef std::map<GtkWidget*, int> PopupMap;

  
  virtual void InfoBubbleClosing(InfoBubbleGtk* info_bubble,
                                 bool closed_by_escape);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void BuildBubble();

  
  CHROMEGTK_CALLBACK_1(ContentSettingBubbleGtk, void, OnPopupIconButtonPress,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_0(ContentSettingBubbleGtk, void, OnPopupLinkClicked);
  CHROMEGTK_CALLBACK_0(ContentSettingBubbleGtk, void, OnRadioToggled);
  CHROMEGTK_CALLBACK_0(ContentSettingBubbleGtk, void, OnCustomLinkClicked);
  CHROMEGTK_CALLBACK_0(ContentSettingBubbleGtk, void, OnManageLinkClicked);
  CHROMEGTK_CALLBACK_0(ContentSettingBubbleGtk, void, OnCloseButtonClicked);

  
  GtkWidget* anchor_;

  
  Profile* profile_;

  
  TabContents* tab_contents_;

  
  NotificationRegistrar registrar_;

  
  InfoBubbleGtkDelegate* delegate_;

  
  scoped_ptr<ContentSettingBubbleModel> content_setting_bubble_model_;

  
  InfoBubbleGtk* info_bubble_;

  
  PopupMap popup_links_;
  PopupMap popup_icons_;

  typedef std::vector<GtkWidget*> RadioGroupGtk;
  RadioGroupGtk radio_group_gtk_;
};

#endif  
