// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_CONTENT_SETTINGS_CONTENT_SETTING_IMAGE_MODEL_H_
#define CHROME_BROWSER_UI_CONTENT_SETTINGS_CONTENT_SETTING_IMAGE_MODEL_H_

#include <string>

#include "base/basictypes.h"
#include "chrome/common/content_settings_types.h"

namespace content {
class WebContents;
}

class ContentSettingImageModel {
 public:
  virtual ~ContentSettingImageModel() {}

  
  static ContentSettingImageModel* CreateContentSettingImageModel(
     ContentSettingsType content_settings_type);

  
  
  virtual void UpdateFromWebContents(content::WebContents* web_contents) = 0;

  ContentSettingsType get_content_settings_type() const {
    return content_settings_type_;
  }
  bool is_visible() const { return is_visible_; }
  int get_icon() const { return icon_; }
  
  
  int explanatory_string_id() const { return explanatory_string_id_; }
  std::string get_tooltip() const { return tooltip_; }

 protected:
  explicit ContentSettingImageModel(ContentSettingsType content_settings_type);
  void set_visible(bool visible) { is_visible_ = visible; }
  void set_icon(int icon) { icon_ = icon; }
  void set_explanatory_string_id(int text_id) {
    explanatory_string_id_ = text_id;
  }
  void set_tooltip(const std::string& tooltip) { tooltip_ = tooltip; }

 private:
  const ContentSettingsType content_settings_type_;
  bool is_visible_;
  int icon_;
  int explanatory_string_id_;
  std::string tooltip_;

  DISALLOW_COPY_AND_ASSIGN(ContentSettingImageModel);
};

#endif  
