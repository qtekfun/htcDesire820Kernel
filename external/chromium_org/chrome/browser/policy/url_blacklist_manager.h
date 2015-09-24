// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_URL_BLACKLIST_MANAGER_H_
#define CHROME_BROWSER_POLICY_URL_BLACKLIST_MANAGER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "components/url_matcher/url_matcher.h"

class GURL;
class PrefService;

namespace base {
class ListValue;
}

namespace net {
class URLRequest;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace policy {

class URLBlacklist {
 public:
  URLBlacklist();
  virtual ~URLBlacklist();

  
  void AddFilters(bool allow, const base::ListValue* filters);

  
  
  
  void Block(const base::ListValue* filters);

  
  
  void Allow(const base::ListValue* filters);

  
  bool IsURLBlocked(const GURL& url) const;

  
  size_t Size() const;

  
  
  
  
  
  
  
  
  
  
  static bool FilterToComponents(const std::string& filter,
                                 std::string* scheme,
                                 std::string* host,
                                 bool* match_subdomains,
                                 uint16* port,
                                 std::string* path);

  
  
  
  static scoped_refptr<url_matcher::URLMatcherConditionSet> CreateConditionSet(
      url_matcher::URLMatcher* url_matcher,
      url_matcher::URLMatcherConditionSet::ID id,
      const std::string& scheme,
      const std::string& host,
      bool match_subdomains,
      uint16 port,
      const std::string& path);

 private:
  struct FilterComponents;

  
  static bool FilterTakesPrecedence(const FilterComponents& lhs,
                                    const FilterComponents& rhs);

  url_matcher::URLMatcherConditionSet::ID id_;
  std::map<url_matcher::URLMatcherConditionSet::ID, FilterComponents> filters_;
  scoped_ptr<url_matcher::URLMatcher> url_matcher_;

  DISALLOW_COPY_AND_ASSIGN(URLBlacklist);
};

class URLBlacklistManager {
 public:
  
  explicit URLBlacklistManager(PrefService* pref_service);
  virtual ~URLBlacklistManager();

  
  void ShutdownOnUIThread();

  
  
  bool IsURLBlocked(const GURL& url) const;

  
  
  
  
  
  bool IsRequestBlocked(const net::URLRequest& request) const;

  
  
  virtual void SetBlacklist(scoped_ptr<URLBlacklist> blacklist);

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

 protected:
  
  
  void ScheduleUpdate();

  
  
  virtual void Update();

  
  
  void UpdateOnIO(scoped_ptr<base::ListValue> block,
                  scoped_ptr<base::ListValue> allow);

 private:
  
  
  

  
  base::WeakPtrFactory<URLBlacklistManager> ui_weak_ptr_factory_;

  
  PrefChangeRegistrar pref_change_registrar_;
  PrefService* pref_service_;  

  
  
  

  
  base::WeakPtrFactory<URLBlacklistManager> io_weak_ptr_factory_;

  
  scoped_ptr<URLBlacklist> blacklist_;

  DISALLOW_COPY_AND_ASSIGN(URLBlacklistManager);
};

}  

#endif  
