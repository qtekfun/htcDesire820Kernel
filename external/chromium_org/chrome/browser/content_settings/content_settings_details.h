// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_DETAILS_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_DETAILS_H_

#include <string>

#include "base/basictypes.h"
#include "chrome/common/content_settings.h"
#include "chrome/common/content_settings_types.h"
#include "chrome/common/content_settings_pattern.h"

class ContentSettingsDetails {
 public:
  
  ContentSettingsDetails(const ContentSettingsPattern& primary_pattern,
                         const ContentSettingsPattern& secondary_pattern,
                         ContentSettingsType type,
                         const std::string& resource_identifier);

  
  const ContentSettingsPattern& primary_pattern() const {
    return primary_pattern_;
  }

  
  const ContentSettingsPattern& secondary_pattern() const {
    return secondary_pattern_;
  }

  
  bool update_all() const {
    return primary_pattern_.ToString().empty() &&
           secondary_pattern_.ToString().empty();
  }

  
  ContentSettingsType type() const { return type_; }

  
  const std::string& resource_identifier() const {
    return resource_identifier_;
  }

  
  
  bool update_all_types() const {
    return CONTENT_SETTINGS_TYPE_DEFAULT == type_;
  }

 private:
  ContentSettingsPattern primary_pattern_;
  ContentSettingsPattern secondary_pattern_;
  ContentSettingsType type_;
  std::string resource_identifier_;

  DISALLOW_COPY_AND_ASSIGN(ContentSettingsDetails);
};

#endif  
