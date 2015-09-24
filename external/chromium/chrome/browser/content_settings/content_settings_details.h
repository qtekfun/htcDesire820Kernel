// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_DETAILS_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_DETAILS_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/content_settings/content_settings_pattern.h"
#include "chrome/common/content_settings.h"

class ContentSettingsDetails {
 public:
  
  ContentSettingsDetails(const ContentSettingsPattern& pattern,
                         ContentSettingsType type,
                         const std::string& resource_identifier)
      : pattern_(pattern),
        type_(type),
        resource_identifier_(resource_identifier) {}

  
  const ContentSettingsPattern& pattern() const { return pattern_; }

  
  bool update_all() const { return pattern_.AsString().empty(); }

  
  ContentSettingsType type() const { return type_; }

  
  const std::string& resource_identifier() const {
    return resource_identifier_;
  }

  
  
  bool update_all_types() const {
    return CONTENT_SETTINGS_TYPE_DEFAULT == type_;
  }

 private:
  ContentSettingsPattern pattern_;
  ContentSettingsType type_;
  std::string resource_identifier_;
};

#endif  
