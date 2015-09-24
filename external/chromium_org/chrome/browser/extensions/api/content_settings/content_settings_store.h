// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_CONTENT_SETTINGS_CONTENT_SETTINGS_STORE_H_
#define CHROME_BROWSER_EXTENSIONS_API_CONTENT_SETTINGS_CONTENT_SETTINGS_STORE_H_

#include <map>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "base/time/time.h"
#include "base/tuple.h"
#include "chrome/browser/content_settings/content_settings_provider.h"
#include "chrome/common/content_settings.h"
#include "chrome/common/content_settings_pattern.h"
#include "extensions/browser/extension_prefs_scope.h"
#include "url/gurl.h"

namespace base {
class ListValue;
}

namespace content_settings {
class OriginIdentifierValueMap;
class RuleIterator;
}

namespace extensions {

class ContentSettingsStore
    : public base::RefCountedThreadSafe<ContentSettingsStore> {
 public:
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    virtual void OnContentSettingChanged(
        const std::string& extension_id,
        bool incognito) = 0;
  };

  ContentSettingsStore();

  

  content_settings::RuleIterator* GetRuleIterator(
      ContentSettingsType type,
      const content_settings::ResourceIdentifier& identifier,
      bool incognito) const;

  
  
  
  
  
  void SetExtensionContentSetting(
      const std::string& ext_id,
      const ContentSettingsPattern& embedded_pattern,
      const ContentSettingsPattern& top_level_pattern,
      ContentSettingsType type,
      const content_settings::ResourceIdentifier& identifier,
      ContentSetting setting,
      ExtensionPrefsScope scope);

  
  void ClearContentSettingsForExtension(const std::string& ext_id,
                                        ExtensionPrefsScope scope);

  
  
  
  base::ListValue* GetSettingsForExtension(const std::string& extension_id,
                                           ExtensionPrefsScope scope) const;

  
  
  void SetExtensionContentSettingFromList(const std::string& extension_id,
                                           const base::ListValue* list,
                                           ExtensionPrefsScope scope);

  

  
  void RegisterExtension(const std::string& ext_id,
                         const base::Time& install_time,
                         bool is_enabled);

  
  void UnregisterExtension(const std::string& ext_id);

  
  
  void SetExtensionState(const std::string& ext_id, bool is_enabled);

  
  void AddObserver(Observer* observer);

  
  void RemoveObserver(Observer* observer);

 private:
  friend class base::RefCountedThreadSafe<ContentSettingsStore>;

  struct ExtensionEntry;

  typedef std::multimap<base::Time, ExtensionEntry*> ExtensionEntryMap;

  virtual ~ContentSettingsStore();

  content_settings::OriginIdentifierValueMap* GetValueMap(
      const std::string& ext_id,
      ExtensionPrefsScope scope);

  const content_settings::OriginIdentifierValueMap* GetValueMap(
      const std::string& ext_id,
      ExtensionPrefsScope scope) const;

  void NotifyOfContentSettingChanged(const std::string& extension_id,
                                     bool incognito);

  bool OnCorrectThread();

  ExtensionEntryMap::iterator FindEntry(const std::string& ext_id);
  ExtensionEntryMap::const_iterator FindEntry(const std::string& ext_id) const;

  ExtensionEntryMap entries_;

  ObserverList<Observer, false> observers_;

  mutable base::Lock lock_;

  DISALLOW_COPY_AND_ASSIGN(ContentSettingsStore);
};

}  

#endif  
