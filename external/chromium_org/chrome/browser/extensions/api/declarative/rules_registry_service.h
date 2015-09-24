// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_RULES_REGISTRY_SERVICE_H__
#define CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_RULES_REGISTRY_SERVICE_H__

#include <map>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "chrome/browser/extensions/api/declarative/rules_registry.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;

namespace content {
class NotificationSource;
class NotificationSource;
}

namespace extensions {
class ContentRulesRegistry;
class RulesRegistry;
class RulesRegistryStorageDelegate;
}

namespace extensions {

class RulesRegistryService : public ProfileKeyedAPI,
                             public content::NotificationObserver {
 public:
  typedef RulesRegistry::WebViewKey WebViewKey;
  struct RulesRegistryKey {
    std::string event_name;
    WebViewKey webview_key;
    RulesRegistryKey(const std::string event_name,
                     const WebViewKey& webview_key)
        : event_name(event_name),
          webview_key(webview_key) {}
    bool operator<(const RulesRegistryKey& other) const {
      return (event_name < other.event_name) ||
          ((event_name == other.event_name) &&
          (webview_key < other.webview_key));
    }
  };

  explicit RulesRegistryService(Profile* profile);
  virtual ~RulesRegistryService();

  
  
  virtual void Shutdown() OVERRIDE;

  
  static ProfileKeyedAPIFactory<RulesRegistryService>* GetFactoryInstance();

  
  static RulesRegistryService* Get(Profile* profile);

  
  void EnsureDefaultRulesRegistriesRegistered(const WebViewKey& webview_key);

  
  void RegisterRulesRegistry(scoped_refptr<RulesRegistry> rule_registry);

  
  
  
  scoped_refptr<RulesRegistry> GetRulesRegistry(
      const WebViewKey& webview_key,
      const std::string& event_name);

  
  ContentRulesRegistry* content_rules_registry() const {
    CHECK(content_rules_registry_);
    return content_rules_registry_;
  }

  
  void RemoveWebViewRulesRegistries(int process_id);

  
  void SimulateExtensionUninstalled(const std::string& extension_id);

 private:
  friend class ProfileKeyedAPIFactory<RulesRegistryService>;

  
  
  typedef std::map<RulesRegistryKey, scoped_refptr<RulesRegistry> >
      RulesRegistryMap;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  
  void NotifyRegistriesHelper(
      void (RulesRegistry::*notification_callback)(const std::string&),
      const std::string& extension_id);

  
  static const char* service_name() {
    return "RulesRegistryService";
  }
  static const bool kServiceHasOwnInstanceInIncognito = true;
  static const bool kServiceIsNULLWhileTesting = true;

  RulesRegistryMap rule_registries_;

  
  ScopedVector<RulesCacheDelegate> cache_delegates_;

  
  
  ContentRulesRegistry* content_rules_registry_;

  content::NotificationRegistrar registrar_;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(RulesRegistryService);
};

}  

#endif  
