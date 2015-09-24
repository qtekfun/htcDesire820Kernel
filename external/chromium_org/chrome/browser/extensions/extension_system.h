// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_SYSTEM_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_SYSTEM_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "extensions/common/extension.h"
#include "extensions/common/one_shot_event.h"

class ExtensionService;
class Profile;

#if defined(OS_CHROMEOS)
namespace chromeos {
class DeviceLocalAccountManagementPolicyProvider;
}
#endif  

namespace content {
class BrowserContext;
}

namespace extensions {
class Blacklist;
class ErrorConsole;
class EventRouter;
class Extension;
class ExtensionSystemSharedFactory;
class ExtensionWarningBadgeService;
class ExtensionWarningService;
class InfoMap;
class InstallVerifier;
class LazyBackgroundTaskQueue;
class ManagementPolicy;
class NavigationObserver;
class ProcessManager;
class StandardManagementPolicyProvider;
class StateStore;
class UserScriptMaster;

class ExtensionSystem : public BrowserContextKeyedService {
 public:
  ExtensionSystem();
  virtual ~ExtensionSystem();

  
  
  static ExtensionSystem* Get(Profile* profile);

  
  static ExtensionSystem* GetForBrowserContext(
      content::BrowserContext* profile);

  
  virtual void Shutdown() OVERRIDE {}

  
  
  
  virtual void InitForRegularProfile(bool extensions_enabled) = 0;

  
  virtual ExtensionService* extension_service() = 0;

  
  
  
  virtual ManagementPolicy* management_policy() = 0;

  
  virtual UserScriptMaster* user_script_master() = 0;

  
  virtual ProcessManager* process_manager() = 0;

  
  virtual StateStore* state_store() = 0;

  
  virtual StateStore* rules_store() = 0;

  
  virtual InfoMap* info_map() = 0;

  
  virtual LazyBackgroundTaskQueue* lazy_background_task_queue() = 0;

  
  virtual EventRouter* event_router() = 0;

  
  virtual ExtensionWarningService* warning_service() = 0;

  
  virtual Blacklist* blacklist() = 0;

  
  virtual ErrorConsole* error_console() = 0;

  
  virtual InstallVerifier* install_verifier() = 0;

  
  
  
  
  
  virtual void RegisterExtensionWithRequestContexts(
      const Extension* extension) {}

  
  
  
  virtual void UnregisterExtensionWithRequestContexts(
      const std::string& extension_id,
      const UnloadedExtensionInfo::Reason reason) {}

  
  virtual const OneShotEvent& ready() const = 0;
};

class ExtensionSystemImpl : public ExtensionSystem {
 public:
  explicit ExtensionSystemImpl(Profile* profile);
  virtual ~ExtensionSystemImpl();

  
  virtual void Shutdown() OVERRIDE;

  virtual void InitForRegularProfile(bool extensions_enabled) OVERRIDE;

  virtual ExtensionService* extension_service() OVERRIDE;  
  virtual ManagementPolicy* management_policy() OVERRIDE;  
  virtual UserScriptMaster* user_script_master() OVERRIDE;  
  virtual ProcessManager* process_manager() OVERRIDE;
  virtual StateStore* state_store() OVERRIDE;  
  virtual StateStore* rules_store() OVERRIDE;  
  virtual LazyBackgroundTaskQueue* lazy_background_task_queue()
      OVERRIDE;  
  virtual InfoMap* info_map() OVERRIDE; 
  virtual EventRouter* event_router() OVERRIDE;  
  virtual ExtensionWarningService* warning_service() OVERRIDE;
  virtual Blacklist* blacklist() OVERRIDE;  
  virtual ErrorConsole* error_console() OVERRIDE;
  virtual InstallVerifier* install_verifier() OVERRIDE;

  virtual void RegisterExtensionWithRequestContexts(
      const Extension* extension) OVERRIDE;

  virtual void UnregisterExtensionWithRequestContexts(
      const std::string& extension_id,
      const UnloadedExtensionInfo::Reason reason) OVERRIDE;

  virtual const OneShotEvent& ready() const OVERRIDE;

 private:
  friend class ExtensionSystemSharedFactory;

  
  
  class Shared : public BrowserContextKeyedService {
   public:
    explicit Shared(Profile* profile);
    virtual ~Shared();

    
    virtual void InitPrefs();
    
    void RegisterManagementPolicyProviders();
    void Init(bool extensions_enabled);

    
    virtual void Shutdown() OVERRIDE;

    StateStore* state_store();
    StateStore* rules_store();
    ExtensionService* extension_service();
    ManagementPolicy* management_policy();
    UserScriptMaster* user_script_master();
    Blacklist* blacklist();
    InfoMap* info_map();
    LazyBackgroundTaskQueue* lazy_background_task_queue();
    EventRouter* event_router();
    ExtensionWarningService* warning_service();
    ErrorConsole* error_console();
    InstallVerifier* install_verifier();
    const OneShotEvent& ready() const { return ready_; }

   private:
    Profile* profile_;

    

    scoped_ptr<StateStore> state_store_;
    scoped_ptr<StateStore> rules_store_;
    
    
    scoped_ptr<LazyBackgroundTaskQueue> lazy_background_task_queue_;
    scoped_ptr<EventRouter> event_router_;
    scoped_ptr<NavigationObserver> navigation_observer_;
    scoped_refptr<UserScriptMaster> user_script_master_;
    scoped_ptr<Blacklist> blacklist_;
    
    scoped_ptr<StandardManagementPolicyProvider>
        standard_management_policy_provider_;
    
    scoped_ptr<ExtensionService> extension_service_;
    scoped_ptr<ManagementPolicy> management_policy_;
    
    scoped_refptr<InfoMap> extension_info_map_;
    scoped_ptr<ExtensionWarningService> extension_warning_service_;
    scoped_ptr<ExtensionWarningBadgeService> extension_warning_badge_service_;
    scoped_ptr<ErrorConsole> error_console_;
    scoped_ptr<InstallVerifier> install_verifier_;

#if defined(OS_CHROMEOS)
    scoped_ptr<chromeos::DeviceLocalAccountManagementPolicyProvider>
        device_local_account_management_policy_provider_;
#endif

    OneShotEvent ready_;
  };

  Profile* profile_;

  Shared* shared_;

  
  
  
  
  scoped_ptr<ProcessManager> process_manager_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionSystemImpl);
};

}  

#endif  
