// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_RULES_CACHE_DELEGATE_H__
#define CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_RULES_CACHE_DELEGATE_H__

#include <set>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/values.h"
#include "content/public/browser/browser_thread.h"

class Profile;

namespace extensions {

class RulesRegistry;

class RulesCacheDelegate {
 public:

  explicit RulesCacheDelegate(bool log_storage_init_delay);

  virtual ~RulesCacheDelegate();

  
  
  
  static std::string GetRulesStoredKey(const std::string& event_name,
                                       bool incognito);

  
  void Init(RulesRegistry* registry);

  void WriteToStorage(const std::string& extension_id,
                      scoped_ptr<base::Value> value);

  base::WeakPtr<RulesCacheDelegate> GetWeakPtr() {
    DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
    return weak_ptr_factory_.GetWeakPtr();
  }

 private:
  FRIEND_TEST_ALL_PREFIXES(RulesRegistryWithCacheTest,
                           DeclarativeRulesStored);
  FRIEND_TEST_ALL_PREFIXES(RulesRegistryWithCacheTest,
                           RulesStoredFlagMultipleRegistries);

  static const char kRulesStoredKey[];

  
  
  
  void CheckIfReady();

  
  
  void ReadRulesForInstalledExtensions();

  
  void ReadFromStorage(const std::string& extension_id);
  void ReadFromStorageCallback(const std::string& extension_id,
                               scoped_ptr<base::Value> value);

  
  
  
  bool GetDeclarativeRulesStored(const std::string& extension_id) const;
  
  void SetDeclarativeRulesStored(const std::string& extension_id,
                                 bool rules_stored);

  Profile* profile_;

  
  std::string storage_key_;

  
  std::string rules_stored_key_;

  
  std::set<std::string> waiting_for_extensions_;

  
  
  base::Time storage_init_time_;
  bool log_storage_init_delay_;

  
  base::WeakPtr<RulesRegistry> registry_;

  
  content::BrowserThread::ID rules_registry_thread_;

  
  bool notified_registry_;

  
  base::WeakPtrFactory<RulesCacheDelegate> weak_ptr_factory_;
};

}  

#endif  
