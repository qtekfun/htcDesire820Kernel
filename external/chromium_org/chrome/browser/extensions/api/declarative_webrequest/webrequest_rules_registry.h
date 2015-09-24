// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_WEBREQUEST_WEBREQUEST_RULES_REGISTRY_H_
#define CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_WEBREQUEST_WEBREQUEST_RULES_REGISTRY_H_

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/extensions/api/declarative/declarative_rule.h"
#include "chrome/browser/extensions/api/declarative/rules_registry.h"
#include "chrome/browser/extensions/api/declarative_webrequest/request_stage.h"
#include "chrome/browser/extensions/api/declarative_webrequest/webrequest_action.h"
#include "chrome/browser/extensions/api/declarative_webrequest/webrequest_condition.h"
#include "components/url_matcher/url_matcher.h"
#include "extensions/browser/info_map.h"

class Profile;
class WebRequestPermissions;

namespace extension_web_request_api_helpers {
struct EventResponseDelta;
}

namespace net {
class URLRequest;
}

namespace extensions {

class RulesRegistryService;

typedef linked_ptr<extension_web_request_api_helpers::EventResponseDelta>
    LinkedPtrEventResponseDelta;
typedef DeclarativeRule<WebRequestCondition, WebRequestAction> WebRequestRule;

class WebRequestRulesRegistry : public RulesRegistry {
 public:
  
  
  WebRequestRulesRegistry(Profile* profile,
                          RulesCacheDelegate* cache_delegate,
                          const WebViewKey& webview_key);

  
  
  std::set<const WebRequestRule*> GetMatches(
      const WebRequestData& request_data_without_ids) const;

  
  
  std::list<LinkedPtrEventResponseDelta> CreateDeltas(
      const InfoMap* extension_info_map,
      const WebRequestData& request_data,
      bool crosses_incognito);

  
  virtual std::string AddRulesImpl(
      const std::string& extension_id,
      const std::vector<linked_ptr<RulesRegistry::Rule> >& rules) OVERRIDE;
  virtual std::string RemoveRulesImpl(
      const std::string& extension_id,
      const std::vector<std::string>& rule_identifiers) OVERRIDE;
  virtual std::string RemoveAllRulesImpl(
      const std::string& extension_id) OVERRIDE;

  
  bool IsEmpty() const;

 protected:
  virtual ~WebRequestRulesRegistry();

  
  virtual base::Time GetExtensionInstallationTime(
      const std::string& extension_id) const;
  virtual void ClearCacheOnNavigation();

  void SetExtensionInfoMapForTesting(
      scoped_refptr<InfoMap> extension_info_map) {
    extension_info_map_ = extension_info_map;
  }

  const std::set<const WebRequestRule*>&
  rules_with_untriggered_conditions_for_test() const {
    return rules_with_untriggered_conditions_;
  }

 private:
  FRIEND_TEST_ALL_PREFIXES(WebRequestRulesRegistrySimpleTest, StageChecker);
  FRIEND_TEST_ALL_PREFIXES(WebRequestRulesRegistrySimpleTest,
                           HostPermissionsChecker);

  typedef std::map<url_matcher::URLMatcherConditionSet::ID, WebRequestRule*>
      RuleTriggers;
  typedef std::map<WebRequestRule::RuleId, linked_ptr<WebRequestRule> >
      RulesMap;
  typedef std::set<url_matcher::URLMatcherConditionSet::ID> URLMatches;
  typedef std::set<const WebRequestRule*> RuleSet;

  
  
  static bool Checker(const Extension* extension,
                      const WebRequestConditionSet* conditions,
                      const WebRequestActionSet* actions,
                      std::string* error);

  
  
  static bool HostPermissionsChecker(const Extension* extension,
                                     const WebRequestActionSet* actions,
                                     std::string* error);

  
  
  static bool StageChecker(const WebRequestConditionSet* conditions,
                           const WebRequestActionSet* actions,
                           std::string* error);

  
  
  
  
  
  
  void CleanUpAfterRule(const WebRequestRule* rule,
                        std::vector<url_matcher::URLMatcherConditionSet::ID>*
                            remove_from_url_matcher);

  
  
  
  void AddTriggeredRules(const URLMatches& url_matches,
                         const WebRequestCondition::MatchData& request_data,
                         RuleSet* result) const;

  
  
  RuleTriggers rule_triggers_;

  
  
  
  std::set<const WebRequestRule*> rules_with_untriggered_conditions_;

  std::map<WebRequestRule::ExtensionId, RulesMap> webrequest_rules_;

  url_matcher::URLMatcher url_matcher_;

  void* profile_id_;
  scoped_refptr<InfoMap> extension_info_map_;

  DISALLOW_COPY_AND_ASSIGN(WebRequestRulesRegistry);
};

}  

#endif  
