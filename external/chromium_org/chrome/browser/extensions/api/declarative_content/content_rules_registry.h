// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_CONTENT_CONTENT_RULES_REGISTRY_H_
#define CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_CONTENT_CONTENT_RULES_REGISTRY_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/extensions/api/declarative/declarative_rule.h"
#include "chrome/browser/extensions/api/declarative/rules_registry.h"
#include "chrome/browser/extensions/api/declarative_content/content_action.h"
#include "chrome/browser/extensions/api/declarative_content/content_condition.h"
#include "components/url_matcher/url_matcher.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/browser/info_map.h"

class Profile;
class ContentPermissions;

namespace content {
class RenderProcessHost;
class WebContents;
struct FrameNavigateParams;
struct LoadCommittedDetails;
}

namespace extension_web_request_api_helpers {
struct EventResponseDelta;
}

namespace net {
class URLRequest;
}

namespace extensions {

class RulesRegistryService;

typedef DeclarativeRule<ContentCondition, ContentAction> ContentRule;

class ContentRulesRegistry : public RulesRegistry,
                             public content::NotificationObserver {
 public:
  
  
  ContentRulesRegistry(Profile* profile, RulesCacheDelegate* cache_delegate);

  
  
  void Apply(content::WebContents* contents,
             const std::vector<std::string>& matching_css_selectors);

  
  void DidNavigateMainFrame(content::WebContents* tab,
                            const content::LoadCommittedDetails& details,
                            const content::FrameNavigateParams& params);

  
  virtual std::string AddRulesImpl(
      const std::string& extension_id,
      const std::vector<linked_ptr<RulesRegistry::Rule> >& rules) OVERRIDE;
  virtual std::string RemoveRulesImpl(
      const std::string& extension_id,
      const std::vector<std::string>& rule_identifiers) OVERRIDE;
  virtual std::string RemoveAllRulesImpl(
      const std::string& extension_id) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  bool IsEmpty() const;

 protected:
  virtual ~ContentRulesRegistry();

  
  virtual base::Time GetExtensionInstallationTime(
      const std::string& extension_id) const;

 private:
  friend class DeclarativeContentRulesRegistryTest;

  std::set<ContentRule*>
  GetMatches(const RendererContentMatchData& renderer_data) const;

  
  
  
  void UpdateConditionCache();

  
  
  void InstructRenderProcess(content::RenderProcessHost* process);

  typedef std::map<url_matcher::URLMatcherConditionSet::ID, ContentRule*>
      URLMatcherIdToRule;
  typedef std::map<ContentRule::GlobalRuleId, linked_ptr<ContentRule> >
      RulesMap;

  
  
  URLMatcherIdToRule match_id_to_rule_;

  RulesMap content_rules_;

  
  
  std::map<int, std::set<ContentRule*> > active_rules_;

  
  url_matcher::URLMatcher url_matcher_;

  
  std::vector<std::string> watched_css_selectors_;

  
  content::NotificationRegistrar registrar_;

  scoped_refptr<InfoMap> extension_info_map_;

  DISALLOW_COPY_AND_ASSIGN(ContentRulesRegistry);
};

}  

#endif  
