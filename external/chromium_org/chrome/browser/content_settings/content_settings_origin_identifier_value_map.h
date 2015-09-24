// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_ORIGIN_IDENTIFIER_VALUE_MAP_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_ORIGIN_IDENTIFIER_VALUE_MAP_H_

#include <map>
#include <string>

#include "base/memory/linked_ptr.h"
#include "chrome/common/content_settings_pattern.h"
#include "chrome/common/content_settings_types.h"

class GURL;

namespace base {
class Lock;
class Value;
}

namespace content_settings {

class RuleIterator;

class OriginIdentifierValueMap {
 public:
  typedef std::string ResourceIdentifier;
  struct EntryMapKey {
    ContentSettingsType content_type;
    ResourceIdentifier resource_identifier;
    EntryMapKey(ContentSettingsType content_type,
                const ResourceIdentifier& resource_identifier);
    bool operator<(const OriginIdentifierValueMap::EntryMapKey& other) const;
  };

  struct PatternPair {
    ContentSettingsPattern primary_pattern;
    ContentSettingsPattern secondary_pattern;
    PatternPair(const ContentSettingsPattern& primary_pattern,
                const ContentSettingsPattern& secondary_pattern);
    bool operator<(const OriginIdentifierValueMap::PatternPair& other) const;
  };

  typedef std::map<PatternPair, linked_ptr<base::Value> > Rules;
  typedef std::map<EntryMapKey, Rules> EntryMap;

  EntryMap::iterator begin() {
    return entries_.begin();
  }

  EntryMap::iterator end() {
    return entries_.end();
  }

  EntryMap::const_iterator begin() const {
    return entries_.begin();
  }

  EntryMap::const_iterator end() const {
    return entries_.end();
  }

  bool empty() const {
    return size() == 0u;
  }

  size_t size() const;

  
  
  
  
  
  
  RuleIterator* GetRuleIterator(ContentSettingsType content_type,
                                const ResourceIdentifier& resource_identifier,
                                base::Lock* lock) const;

  OriginIdentifierValueMap();
  ~OriginIdentifierValueMap();

  
  
  
  base::Value* GetValue(
      const GURL& primary_url,
      const GURL& secondary_url,
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier) const;

  
  
  
  void SetValue(
      const ContentSettingsPattern& primary_pattern,
      const ContentSettingsPattern& secondary_pattern,
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier,
      base::Value* value);

  
  
  void DeleteValue(
      const ContentSettingsPattern& primary_pattern,
      const ContentSettingsPattern& secondary_pattern,
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier);

  
  
  void DeleteValues(
      ContentSettingsType content_type,
      const ResourceIdentifier& resource_identifier);

  
  void clear();

 private:
  EntryMap entries_;

  DISALLOW_COPY_AND_ASSIGN(OriginIdentifierValueMap);
};

}  

#endif  
