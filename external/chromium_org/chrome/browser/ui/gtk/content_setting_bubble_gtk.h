// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_CONTENT_SETTING_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_CONTENT_SETTING_BUBBLE_GTK_H_

#include <map>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/gtk/bubble/bubble_gtk.h"
#include "chrome/browser/ui/gtk/menu_gtk.h"
#include "chrome/common/content_settings_types.h"
#include "content/public/common/media_stream_request.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"

class ContentSettingBubbleModel;
class ContentSettingMediaMenuModel;
class Profile;

namespace ui {
class SimpleMenuModel;
}

class ContentSettingBubbleGtk : public BubbleDelegateGtk {
 public:
   ContentSettingBubbleGtk(
       GtkWidget* anchor,
       BubbleDelegateGtk* delegate,
       ContentSettingBubbleModel* content_setting_bubble_model,
       Profile* profile);
  virtual ~ContentSettingBubbleGtk();

  
  void UpdateMenuLabel(content::MediaStreamType type,
                       const std::string& label);

  
  void Close();

 private:
  
  
  struct MediaMenuGtk {
    explicit MediaMenuGtk(content::MediaStreamType type);
    ~MediaMenuGtk();

    content::MediaStreamType type;
    scoped_ptr<ui::SimpleMenuModel> menu_model;
    scoped_ptr<MenuGtk> menu;
    ui::OwnedWidgetGtk label;

   private:
    DISALLOW_COPY_AND_ASSIGN(MediaMenuGtk);
  };
  typedef std::map<GtkWidget*, MediaMenuGtk*> GtkMediaMenuMap;

  typedef std::map<GtkWidget*, int> PopupMap;

  
  virtual void BubbleClosing(BubbleGtk* bubble, bool closed_by_escape) OVERRIDE;

  
  void BuildBubble();

  
  CHROMEGTK_CALLBACK_1(ContentSettingBubbleGtk, void, OnPopupIconButtonPress,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_0(ContentSettingBubbleGtk, void, OnPopupLinkClicked);
  CHROMEGTK_CALLBACK_0(ContentSettingBubbleGtk, void, OnRadioToggled);
  CHROMEGTK_CALLBACK_0(ContentSettingBubbleGtk, void, OnCustomLinkClicked);
  CHROMEGTK_CALLBACK_0(ContentSettingBubbleGtk, void, OnManageLinkClicked);
  CHROMEGTK_CALLBACK_0(ContentSettingBubbleGtk, void, OnCloseButtonClicked);
  CHROMEGTK_CALLBACK_0(ContentSettingBubbleGtk, void, OnMenuButtonClicked);

  
  GtkWidget* anchor_;

  
  Profile* profile_;

  
  BubbleDelegateGtk* delegate_;

  
  scoped_ptr<ContentSettingBubbleModel> content_setting_bubble_model_;

  
  BubbleGtk* bubble_;

  
  PopupMap popup_links_;
  PopupMap popup_icons_;

  typedef std::vector<GtkWidget*> RadioGroupGtk;
  RadioGroupGtk radio_group_gtk_;

  GtkMediaMenuMap media_menus_;
};

#endif  
