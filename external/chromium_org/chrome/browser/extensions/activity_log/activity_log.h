// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_ACTIVITY_LOG_H_
#define CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_ACTIVITY_LOG_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/singleton.h"
#include "base/observer_list_threadsafe.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread.h"
#include "chrome/browser/extensions/activity_log/activity_actions.h"
#include "chrome/browser/extensions/activity_log/activity_database.h"
#include "chrome/browser/extensions/activity_log/activity_log_policy.h"
#include "chrome/browser/extensions/install_observer.h"
#include "chrome/browser/extensions/install_tracker.h"
#include "chrome/browser/extensions/tab_helper.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/extensions/dom_action_types.h"
#include "components/browser_context_keyed_service/browser_context_dependency_manager.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"
#include "extensions/browser/event_router.h"

class Profile;
using content::BrowserThread;

namespace content {
class BrowserContext;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace extensions {
class Extension;
class ActivityLogPolicy;

class ActivityLog : public BrowserContextKeyedService,
                    public EventRouter::EventDispatchObserver,
                    public TabHelper::ScriptExecutionObserver,
                    public InstallObserver {
 public:
  
  
  class Observer {
   public:
    virtual void OnExtensionActivity(scoped_refptr<Action> activity) = 0;
  };

  
  
  static ActivityLog* GetInstance(content::BrowserContext* context);

  
  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  
  
  void LogAction(scoped_refptr<Action> action);

  
  
  
  
  void GetFilteredActions(
      const std::string& extension_id,
      const Action::ActionType type,
      const std::string& api_name,
      const std::string& page_url,
      const std::string& arg_url,
      const int days_ago,
      const base::Callback
          <void(scoped_ptr<std::vector<scoped_refptr<Action> > >)>& callback);

  
  
  
  virtual void OnExtensionInstalled(const Extension* extension) OVERRIDE {}
  virtual void OnExtensionLoaded(const Extension* extension) OVERRIDE;
  virtual void OnExtensionUnloaded(const Extension* extension) OVERRIDE;
  virtual void OnExtensionUninstalled(const Extension* extension) OVERRIDE;
  
  virtual void OnBeginExtensionInstall(
      const ExtensionInstallParams& params) OVERRIDE {}
  virtual void OnDownloadProgress(const std::string& extension_id,
                                  int percent_downloaded) OVERRIDE {}
  virtual void OnInstallFailure(const std::string& extension_id) OVERRIDE {}
  virtual void OnAppsReordered() OVERRIDE {}
  virtual void OnAppInstalledToAppList(
      const std::string& extension_id) OVERRIDE {}
  virtual void OnShutdown() OVERRIDE {}

  
  virtual void OnWillDispatchEvent(scoped_ptr<EventDispatchInfo> details)
      OVERRIDE;

  
  virtual void Shutdown() OVERRIDE;

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  
  
  void RemoveURLs(const std::vector<GURL>& restrict_urls);
  void RemoveURLs(const std::set<GURL>& restrict_urls);
  void RemoveURL(const GURL& url);

  
  void DeleteDatabase();

 private:
  friend class ActivityLogFactory;
  friend class ActivityLogTest;
  friend class RenderViewActivityLogTest;

  explicit ActivityLog(Profile* profile);
  virtual ~ActivityLog();

  
  
  bool IsWatchdogAppActive();
  
  
  void SetWatchdogAppActive(bool active);

  
  
  
  bool IsDatabaseEnabled();

  
  
  void InitInstallTracker();

  
  
  virtual void OnScriptsExecuted(
      const content::WebContents* web_contents,
      const ExecutingScriptsMap& extension_ids,
      int32 page_id,
      const GURL& on_url) OVERRIDE;

  
  
  
  
  void ChooseDatabasePolicy();
  void SetDatabasePolicy(ActivityLogPolicy::PolicyType policy_type);

  typedef ObserverListThreadSafe<Observer> ObserverList;
  scoped_refptr<ObserverList> observers_;

  
  
  
  

  
  
  
  ActivityLogDatabasePolicy* database_policy_;
  ActivityLogPolicy::PolicyType database_policy_type_;

  
  
  ActivityLogPolicy* uma_policy_;

  Profile* profile_;
  bool db_enabled_;  
  
  
  
  
  
  bool testing_mode_;
  
  
  
  
  bool has_threads_;

  
  
  InstallTracker* tracker_;

  
  
  
  int watchdog_apps_active_;

  FRIEND_TEST_ALL_PREFIXES(ActivityLogApiTest, TriggerEvent);
  FRIEND_TEST_ALL_PREFIXES(ActivityLogEnabledTest, AppAndCommandLine);
  FRIEND_TEST_ALL_PREFIXES(ActivityLogEnabledTest, CommandLineSwitch);
  FRIEND_TEST_ALL_PREFIXES(ActivityLogEnabledTest, NoSwitch);
  FRIEND_TEST_ALL_PREFIXES(ActivityLogEnabledTest, PrefSwitch);
  FRIEND_TEST_ALL_PREFIXES(ActivityLogEnabledTest, WatchdogSwitch);
  DISALLOW_COPY_AND_ASSIGN(ActivityLog);
};

class ActivityLogFactory : public BrowserContextKeyedServiceFactory {
 public:
  static ActivityLog* GetForBrowserContext(content::BrowserContext* context) {
    return static_cast<ActivityLog*>(
        GetInstance()->GetServiceForBrowserContext(context, true));
  }

  static ActivityLogFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<ActivityLogFactory>;
  ActivityLogFactory();
  virtual ~ActivityLogFactory();

  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;

  virtual content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ActivityLogFactory);
};


}  

#endif  
