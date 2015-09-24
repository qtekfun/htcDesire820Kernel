// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_PROVIDER_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_PROVIDER_H_

#define NO_RESOURCE_IDENTIFIER std::string()

#include <string>
#include <vector>

#include "base/values.h"
#include "chrome/common/content_settings_types.h"

class ContentSettingsPattern;

namespace content_settings {

struct Rule;
class RuleIterator;

typedef std::string ResourceIdentifier;

class ProviderInterface {
 public:
  virtual ~ProviderInterface() {}

  
  
  
  
  
  
  
  
  virtual RuleIterator* GetRuleIterator(
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier,
      bool incognito) const = 0;

  
  
  
  
  
  
  
  
  virtual bool SetWebsiteSetting(
      const ContentSettingsPattern& primary_pattern,
      const ContentSettingsPattern& secondary_pattern,
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier,
      base::Value* value) = 0;

  
  
  
  
  
  virtual void ClearAllContentSettingsRules(
      ContentSettingsType content_type) = 0;

  
  
  
  
  virtual void ShutdownOnUIThread() = 0;
};

}  

#endif  
