// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MANAGED_MODE_MANAGED_USER_SERVICE_H_
#define CHROME_BROWSER_MANAGED_MODE_MANAGED_USER_SERVICE_H_

#include <set>
#include <vector>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/strings/string16.h"
#include "chrome/browser/managed_mode/managed_mode_url_filter.h"
#include "chrome/browser/managed_mode/managed_users.h"
#include "chrome/browser/sync/profile_sync_service_observer.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents.h"
#include "extensions/browser/management_policy.h"

class Browser;
class GoogleServiceAuthError;
class ManagedModeURLFilter;
class ManagedModeSiteList;
class ManagedUserRegistrationUtility;
class ManagedUserSettingsService;
class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

class ManagedUserService : public BrowserContextKeyedService,
                           public extensions::ManagementPolicy::Provider,
                           public ProfileSyncServiceObserver,
                           public content::NotificationObserver,
                           public chrome::BrowserListObserver {
 public:
  typedef std::vector<base::string16> CategoryList;
  typedef base::Callback<void(content::WebContents*)> NavigationBlockedCallback;
  typedef base::Callback<void(const GoogleServiceAuthError&)> AuthErrorCallback;

  enum ManualBehavior {
    MANUAL_NONE = 0,
    MANUAL_ALLOW,
    MANUAL_BLOCK
  };

  virtual ~ManagedUserService();

  
  virtual void Shutdown() OVERRIDE;

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  static void MigrateUserPrefs(PrefService* prefs);

  
  
  scoped_refptr<const ManagedModeURLFilter> GetURLFilterForIOThread();

  
  
  ManagedModeURLFilter* GetURLFilterForUIThread();

  
  int GetCategory(const GURL& url);

  
  
  
  void GetCategoryNames(CategoryList* list);

  
  bool AccessRequestsEnabled();

  
  
  
  void AddAccessRequest(const GURL& url);

  
  std::string GetCustodianEmailAddress() const;

  
  
  std::string GetCustodianName() const;

  
  
  

  
  ManualBehavior GetManualBehaviorForHost(const std::string& hostname);

  
  ManualBehavior GetManualBehaviorForURL(const GURL& url);

  
  void GetManualExceptionsForHost(const std::string& host,
                                  std::vector<GURL>* urls);

  
  
  void Init();

  
  
  void InitSync(const std::string& refresh_token);

  
  
  
  
  
  void RegisterAndInitSync(ManagedUserRegistrationUtility* registration_utility,
                           Profile* custodian_profile,
                           const std::string& managed_user_id,
                           const AuthErrorCallback& callback);

  void set_elevated_for_testing(bool skip) {
    elevated_for_testing_ = skip;
  }

  void AddNavigationBlockedCallback(const NavigationBlockedCallback& callback);
  void DidBlockNavigation(content::WebContents* web_contents);

  
  virtual std::string GetDebugPolicyProviderName() const OVERRIDE;
  virtual bool UserMayLoad(const extensions::Extension* extension,
                           base::string16* error) const OVERRIDE;
  virtual bool UserMayModifySettings(const extensions::Extension* extension,
                                     base::string16* error) const OVERRIDE;

  
  virtual void OnStateChanged() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnBrowserSetLastActive(Browser* browser) OVERRIDE;

 private:
  friend class ManagedUserServiceExtensionTestBase;
  friend class ManagedUserServiceFactory;
  FRIEND_TEST_ALL_PREFIXES(ManagedUserServiceTest,
                           ExtensionManagementPolicyProviderUnmanaged);
  FRIEND_TEST_ALL_PREFIXES(ManagedUserServiceTest,
                           ExtensionManagementPolicyProviderManaged);

  
  
  
  class URLFilterContext {
   public:
    URLFilterContext();
    ~URLFilterContext();

    ManagedModeURLFilter* ui_url_filter() const;
    ManagedModeURLFilter* io_url_filter() const;

    void SetDefaultFilteringBehavior(
        ManagedModeURLFilter::FilteringBehavior behavior);
    void LoadWhitelists(ScopedVector<ManagedModeSiteList> site_lists);
    void SetManualHosts(scoped_ptr<std::map<std::string, bool> > host_map);
    void SetManualURLs(scoped_ptr<std::map<GURL, bool> > url_map);

   private:
    
    
    
    
    
    
    scoped_refptr<ManagedModeURLFilter> ui_url_filter_;
    scoped_refptr<ManagedModeURLFilter> io_url_filter_;

    DISALLOW_COPY_AND_ASSIGN(URLFilterContext);
  };

  
  
  explicit ManagedUserService(Profile* profile);

  void OnCustodianProfileDownloaded(const base::string16& full_name);

  void OnManagedUserRegistered(const AuthErrorCallback& callback,
                               Profile* custodian_profile,
                               const GoogleServiceAuthError& auth_error,
                               const std::string& token);

  void SetupSync();

  bool ProfileIsManaged() const;

  
  
  
  bool ExtensionManagementPolicyImpl(const extensions::Extension* extension,
                                     base::string16* error) const;

  
  
  ScopedVector<ManagedModeSiteList> GetActiveSiteLists();

  ManagedUserSettingsService* GetSettingsService();

  void OnDefaultFilteringBehaviorChanged();

  void UpdateSiteLists();

  
  
  void UpdateManualHosts();

  
  
  void UpdateManualURLs();

  
  
  
  
  void RecordProfileAndBrowserEventsHelper(const char* key_prefix);

  
  Profile* profile_;

  content::NotificationRegistrar registrar_;
  PrefChangeRegistrar pref_change_registrar_;

  
  bool waiting_for_sync_initialization_;
  bool is_profile_active_;

  std::vector<NavigationBlockedCallback> navigation_blocked_callbacks_;

  
  bool elevated_for_testing_;

  
  bool did_shutdown_;

  URLFilterContext url_filter_context_;

  base::WeakPtrFactory<ManagedUserService> weak_ptr_factory_;
};

#endif  
