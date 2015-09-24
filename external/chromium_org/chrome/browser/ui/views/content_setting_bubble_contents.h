// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_CONTENT_SETTING_BUBBLE_CONTENTS_H_
#define CHROME_BROWSER_UI_VIEWS_CONTENT_SETTING_BUBBLE_CONTENTS_H_

#include <map>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/common/content_settings_types.h"
#include "content/public/common/media_stream_request.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/button/menu_button_listener.h"
#include "ui/views/controls/link_listener.h"

class ContentSettingBubbleModel;
class ContentSettingMediaMenuModel;
class Profile;

namespace ui {
class SimpleMenuModel;
}

namespace views {
class LabelButton;
class MenuButton;
class MenuRunner;
class RadioButton;
}

class ContentSettingBubbleContents : public views::BubbleDelegateView,
                                     public views::ButtonListener,
                                     public views::LinkListener,
                                     public views::MenuButtonListener {
 public:
  ContentSettingBubbleContents(
      ContentSettingBubbleModel* content_setting_bubble_model,
      views::View* anchor_view,
      views::BubbleBorder::Arrow arrow);
  virtual ~ContentSettingBubbleContents();

  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  void UpdateMenuLabel(content::MediaStreamType type,
                       const std::string& label);

 protected:
  
  virtual void Init() OVERRIDE;

 private:
  class Favicon;
  struct MediaMenuParts;

  typedef std::map<views::Link*, int> PopupLinks;
  typedef std::map<views::MenuButton*, MediaMenuParts*> MediaMenuPartsMap;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  
  virtual void OnMenuButtonClicked(views::View* source,
                                   const gfx::Point& point) OVERRIDE;

  
  int GetPreferredMediaMenuWidth(views::MenuButton* button,
                                 ui::SimpleMenuModel* menu_model);

  
  scoped_ptr<ContentSettingBubbleModel> content_setting_bubble_model_;

  
  
  PopupLinks popup_links_;
  typedef std::vector<views::RadioButton*> RadioGroup;
  RadioGroup radio_group_;
  views::Link* custom_link_;
  views::Link* manage_link_;
  views::LabelButton* close_button_;
  scoped_ptr<views::MenuRunner> menu_runner_;
  MediaMenuPartsMap media_menus_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ContentSettingBubbleContents);
};

#endif  
