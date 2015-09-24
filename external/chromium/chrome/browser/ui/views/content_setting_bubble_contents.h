// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_CONTENT_SETTING_BUBBLE_CONTENTS_H_
#define CHROME_BROWSER_UI_VIEWS_CONTENT_SETTING_BUBBLE_CONTENTS_H_
#pragma once

#include <map>

#include "chrome/common/content_settings_types.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "views/controls/button/button.h"
#include "views/controls/link.h"


class Bubble;
class ContentSettingBubbleModel;
class Profile;
class TabContents;

namespace views {
class NativeButton;
class RadioButton;
}

class ContentSettingBubbleContents : public views::View,
                                     public views::ButtonListener,
                                     public views::LinkController,
                                     public NotificationObserver {
 public:
  ContentSettingBubbleContents(
      ContentSettingBubbleModel* content_setting_bubble_model,
      Profile* profile, TabContents* tab_contents);
  virtual ~ContentSettingBubbleContents();

  
  
  void set_bubble(Bubble* bubble) { bubble_ = bubble; }

  virtual gfx::Size GetPreferredSize();

 private:
  class Favicon;

  typedef std::map<views::Link*, int> PopupLinks;

  
  virtual void ViewHierarchyChanged(bool is_add, View* parent, View* child);

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

  
  virtual void LinkActivated(views::Link* source, int event_flags);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void InitControlLayout();

  
  scoped_ptr<ContentSettingBubbleModel> content_setting_bubble_model_;

  
  Profile* profile_;

  
  TabContents* tab_contents_;

  
  NotificationRegistrar registrar_;

  
  Bubble* bubble_;

  
  
  PopupLinks popup_links_;
  typedef std::vector<views::RadioButton*> RadioGroup;
  RadioGroup radio_group_;
  views::Link* custom_link_;
  views::Link* manage_link_;
  views::NativeButton* close_button_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ContentSettingBubbleContents);
};

#endif  
