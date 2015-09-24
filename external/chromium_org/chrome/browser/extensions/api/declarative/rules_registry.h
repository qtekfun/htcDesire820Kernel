// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_RULES_REGISTRY_H__
#define CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_RULES_REGISTRY_H__

#include "chrome/browser/extensions/api/declarative/rules_registry.h"

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/common/extensions/api/events.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/common/one_shot_event.h"

class Profile;

namespace base {
class Value;
}  

namespace extensions {

class RulesCacheDelegate;

class RulesRegistry : public base::RefCountedThreadSafe<RulesRegistry> {
 public:
  typedef extensions::api::events::Rule Rule;
  struct WebViewKey {
    int embedder_process_id;
    int webview_instance_id;
    WebViewKey(int embedder_process_id, int webview_instance_id)
        : embedder_process_id(embedder_process_id),
          webview_instance_id(webview_instance_id) {}
    bool operator<(const WebViewKey& other) const {
      return embedder_process_id < other.embedder_process_id ||
          ((embedder_process_id == other.embedder_process_id) &&
           (webview_instance_id < other.webview_instance_id));
    }
  };

  enum Defaults { DEFAULT_PRIORITY = 100 };
  
  
  
  
  
  RulesRegistry(Profile* profile,
                const std::string& event_name,
                content::BrowserThread::ID owner_thread,
                RulesCacheDelegate* cache_delegate,
                const WebViewKey& webview_key);

  const OneShotEvent& ready() const {
    return ready_;
  }

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  std::string AddRules(
      const std::string& extension_id,
      const std::vector<linked_ptr<RulesRegistry::Rule> >& rules);

  
  
  
  
  
  
  
  
  
  
  std::string RemoveRules(
      const std::string& extension_id,
      const std::vector<std::string>& rule_identifiers);

  
  std::string RemoveAllRules(const std::string& extension_id);

  
  
  
  
  
  void GetRules(const std::string& extension_id,
                const std::vector<std::string>& rule_identifiers,
                std::vector<linked_ptr<RulesRegistry::Rule> >* out);

  
  void GetAllRules(const std::string& extension_id,
                   std::vector<linked_ptr<RulesRegistry::Rule> >* out);

  
  
  void OnExtensionUnloaded(const std::string& extension_id);
  void OnExtensionUninstalled(const std::string& extension_id);
  void OnExtensionLoaded(const std::string& extension_id);

  
  
  size_t GetNumberOfUsedRuleIdentifiersForTesting() const;

  
  RulesCacheDelegate* rules_cache_delegate_for_testing() const {
    return cache_delegate_.get();
  }

  
  Profile* profile() const { return profile_; }

  
  
  content::BrowserThread::ID owner_thread() const { return owner_thread_; }

  
  const std::string& event_name() const { return event_name_; }

  
  
  const WebViewKey& webview_key() const {
    return webview_key_;
  }

 protected:
  virtual ~RulesRegistry();

  
  
  
  
  
  
  
  std::string AddRulesNoFill(
      const std::string& extension_id,
      const std::vector<linked_ptr<RulesRegistry::Rule> >& rules);

  
  
  
  virtual std::string AddRulesImpl(
      const std::string& extension_id,
      const std::vector<linked_ptr<RulesRegistry::Rule> >& rules) = 0;
  virtual std::string RemoveRulesImpl(
      const std::string& extension_id,
      const std::vector<std::string>& rule_identifiers) = 0;
  virtual std::string RemoveAllRulesImpl(
      const std::string& extension_id) = 0;

 private:
  friend class base::RefCountedThreadSafe<RulesRegistry>;
  friend class RulesCacheDelegate;

  typedef std::string ExtensionId;
  typedef std::string RuleId;
  typedef std::pair<ExtensionId, RuleId> RulesDictionaryKey;
  typedef std::map<RulesDictionaryKey, linked_ptr<RulesRegistry::Rule> >
      RulesDictionary;
  enum ProcessChangedRulesState {
    
    NEVER_PROCESS,
    
    SCHEDULED_FOR_PROCESSING,
    
    
    NOT_SCHEDULED_FOR_PROCESSING
  };
  typedef std::map<ExtensionId, ProcessChangedRulesState> ProcessStateMap;

  base::WeakPtr<RulesRegistry> GetWeakPtr() {
    DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
    return weak_ptr_factory_.GetWeakPtr();
  }

  
  void ProcessChangedRules(const std::string& extension_id);

  
  
  
  void MaybeProcessChangedRules(const std::string& extension_id);

  
  
  
  
  std::string RemoveAllRulesNoStoreUpdate(const std::string& extension_id);

  void MarkReady(base::Time storage_init_time);

  
  
  void DeserializeAndAddRules(const std::string& extension_id,
                              scoped_ptr<base::Value> rules);

  
  Profile* profile_;

  
  const content::BrowserThread::ID owner_thread_;

  
  const std::string event_name_;

  
  WebViewKey webview_key_;

  RulesDictionary rules_;

  
  
  OneShotEvent ready_;

  
  
  base::WeakPtrFactory<RulesRegistry> weak_ptr_factory_;

  
  
  
  
  
  
  
  base::WeakPtr<RulesCacheDelegate> cache_delegate_;

  ProcessStateMap process_changed_rules_requested_;

  
  
  bool IsUniqueId(const std::string& extension_id,
                  const std::string& rule_id) const;

  
  std::string GenerateUniqueId(const std::string& extension_id);

  
  
  
  std::string CheckAndFillInOptionalRules(
    const std::string& extension_id,
    const std::vector<linked_ptr<RulesRegistry::Rule> >& rules);

  
  void FillInOptionalPriorities(
      const std::vector<linked_ptr<RulesRegistry::Rule> >& rules);

  
  void RemoveUsedRuleIdentifiers(const std::string& extension_id,
                                 const std::vector<std::string>& identifiers);

  
  
  void RemoveAllUsedRuleIdentifiers(const std::string& extension_id);

  typedef std::string RuleIdentifier;
  typedef std::map<ExtensionId, std::set<RuleIdentifier> > RuleIdentifiersMap;
  RuleIdentifiersMap used_rule_identifiers_;
  int last_generated_rule_identifier_id_;

  DISALLOW_COPY_AND_ASSIGN(RulesRegistry);
};

}  

#endif  
