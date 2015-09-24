// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_PROFILES_PROFILE_IMPL_H_
#define CHROME_BROWSER_PROFILES_PROFILE_IMPL_H_

#include <string>

#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/timer/timer.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_impl_io_data.h"
#include "content/public/browser/content_browser_client.h"
#include "content/public/browser/host_zoom_map.h"

class NetPrefObserver;
class PrefService;
class PrefServiceSyncable;
class SSLConfigServiceManager;

#if defined(OS_CHROMEOS)
namespace chromeos {
class LocaleChangeGuard;
class Preferences;
}
#endif

namespace base {
class SequencedTaskRunner;
}

namespace extensions {
class ExtensionSystem;
}

namespace policy {
class CloudPolicyManager;
class ProfilePolicyConnector;
class SchemaRegistryService;
}

namespace user_prefs {
class refRegistrySyncable;
}

class ProfileImpl : public Profile {
 public:
  // Value written to prefs when the exit type is EXIT_NORMAL. Public for tests.
  static const char* const kPrefExitTypeNormal;

  virtual ~ProfileImpl();

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  virtual base::FilePath GetPath() const OVERRIDE;
  virtual content::DownloadManagerDelegate*
      GetDownloadManagerDelegate() OVERRIDE;
  virtual net::URLRequestContextGetter* GetRequestContext() OVERRIDE;
  virtual net::URLRequestContextGetter* GetRequestContextForRenderProcess(
      int renderer_child_id) OVERRIDE;
  virtual net::URLRequestContextGetter* GetMediaRequestContext() OVERRIDE;
  virtual net::URLRequestContextGetter* GetMediaRequestContextForRenderProcess(
      int renderer_child_id) OVERRIDE;
  virtual net::URLRequestContextGetter*
      GetMediaRequestContextForStoragePartition(
          const base::FilePath& partition_path,
          bool in_memory) OVERRIDE;
  virtual void RequestMIDISysExPermission(
      int render_process_id,
      int render_view_id,
      int bridge_id,
      const GURL& requesting_frame,
      const MIDISysExPermissionCallback& callback) OVERRIDE;
  virtual void CancelMIDISysExPermissionRequest(
      int render_process_id,
      int render_view_id,
      int bridge_id,
      const GURL& requesting_frame) OVERRIDE;
  virtual content::ResourceContext* GetResourceContext() OVERRIDE;
  virtual content::GeolocationPermissionContext*
      GetGeolocationPermissionContext() OVERRIDE;
  virtual quota::SpecialStoragePolicy* GetSpecialStoragePolicy() OVERRIDE;

  
  virtual scoped_refptr<base::SequencedTaskRunner> GetIOTaskRunner() OVERRIDE;
  
  
  virtual std::string GetProfileName() OVERRIDE;
  virtual bool IsOffTheRecord() const OVERRIDE;
  virtual Profile* GetOffTheRecordProfile() OVERRIDE;
  virtual void DestroyOffTheRecordProfile() OVERRIDE;
  virtual bool HasOffTheRecordProfile() OVERRIDE;
  virtual Profile* GetOriginalProfile() OVERRIDE;
  virtual bool IsManaged() OVERRIDE;
  virtual history::TopSites* GetTopSites() OVERRIDE;
  virtual history::TopSites* GetTopSitesWithoutCreating() OVERRIDE;
  virtual ExtensionService* GetExtensionService() OVERRIDE;
  virtual ExtensionSpecialStoragePolicy*
      GetExtensionSpecialStoragePolicy() OVERRIDE;
  virtual PrefService* GetPrefs() OVERRIDE;
  virtual PrefService* GetOffTheRecordPrefs() OVERRIDE;
  virtual net::URLRequestContextGetter*
      GetRequestContextForExtensions() OVERRIDE;
  virtual net::SSLConfigService* GetSSLConfigService() OVERRIDE;
  virtual HostContentSettingsMap* GetHostContentSettingsMap() OVERRIDE;
  virtual bool IsSameProfile(Profile* profile) OVERRIDE;
  virtual base::Time GetStartTime() const OVERRIDE;
  virtual net::URLRequestContextGetter* CreateRequestContext(
      content::ProtocolHandlerMap* protocol_handlers) OVERRIDE;
  virtual net::URLRequestContextGetter* CreateRequestContextForStoragePartition(
      const base::FilePath& partition_path,
      bool in_memory,
      content::ProtocolHandlerMap* protocol_handlers) OVERRIDE;
  virtual base::FilePath last_selected_directory() OVERRIDE;
  virtual void set_last_selected_directory(const base::FilePath& path) OVERRIDE;
  virtual chrome_browser_net::Predictor* GetNetworkPredictor() OVERRIDE;
  virtual void ClearNetworkingHistorySince(
      base::Time time,
      const base::Closure& completion) OVERRIDE;
  virtual GURL GetHomePage() OVERRIDE;
  virtual bool WasCreatedByVersionOrLater(const std::string& version) OVERRIDE;
  virtual void SetExitType(ExitType exit_type) OVERRIDE;
  virtual ExitType GetLastSessionExitType() OVERRIDE;

#if defined(OS_CHROMEOS)
  virtual void ChangeAppLocale(const std::string& locale,
                               AppLocaleChangedVia) OVERRIDE;
  virtual void OnLogin() OVERRIDE;
  virtual void InitChromeOSPreferences() OVERRIDE;
#endif  

  virtual PrefProxyConfigTracker* GetProxyConfigTracker() OVERRIDE;

 private:
  friend class Profile;
  friend class BetterSessionRestoreCrashTest;
  FRIEND_TEST_ALL_PREFIXES(StartupBrowserCreatorTest,
                           ProfilesLaunchedAfterCrash);
  FRIEND_TEST_ALL_PREFIXES(ProfileBrowserTest, ProfileReadmeCreated);
  FRIEND_TEST_ALL_PREFIXES(ProfileBrowserTest,
                           ProfileDeletedBeforeReadmeCreated);

  
  
  static int create_readme_delay_ms;

  ProfileImpl(const base::FilePath& path,
              Delegate* delegate,
              CreateMode create_mode,
              base::SequencedTaskRunner* sequenced_task_runner);

  
  void DoFinalInit();

  void InitHostZoomMap();

  void OnDefaultZoomLevelChanged();
  void OnZoomLevelChanged(
      const content::HostZoomMap::ZoomLevelChange& change);

  
  void OnPrefsLoaded(bool success);

  base::FilePath GetPrefFilePath();

#if defined(ENABLE_SESSION_SERVICE)
  void StopCreateSessionServiceTimer();

  void EnsureSessionServiceCreated();
#endif


  void EnsureRequestContextCreated() {
    GetRequestContext();
  }

  void UpdateProfileUserNameCache();

  
  void UpdateProfileNameCache();
  void UpdateProfileAvatarCache();
  void UpdateProfileIsEphemeralCache();

  void GetCacheParameters(bool is_media_context,
                          base::FilePath* cache_path,
                          int* max_size);

  PrefProxyConfigTracker* CreateProxyConfigTracker();

  scoped_ptr<content::HostZoomMap::Subscription> zoom_subscription_;
  PrefChangeRegistrar pref_change_registrar_;

  base::FilePath path_;
  base::FilePath base_cache_path_;

  
  
  
  
  

  
  
  
  
  
#if defined(ENABLE_CONFIGURATION_POLICY)
  scoped_ptr<policy::SchemaRegistryService> schema_registry_service_;
  scoped_ptr<policy::CloudPolicyManager> cloud_policy_manager_;
#endif
  scoped_ptr<policy::ProfilePolicyConnector> profile_policy_connector_;

  
  
  
  scoped_refptr<user_prefs::PrefRegistrySyncable> pref_registry_;
  scoped_ptr<PrefServiceSyncable> prefs_;
  scoped_ptr<PrefServiceSyncable> otr_prefs_;
  ProfileImplIOData::Handle io_data_;
  scoped_refptr<ExtensionSpecialStoragePolicy>
      extension_special_storage_policy_;
  scoped_ptr<NetPrefObserver> net_pref_observer_;
  scoped_ptr<SSLConfigServiceManager> ssl_config_service_manager_;
  scoped_refptr<HostContentSettingsMap> host_content_settings_map_;
  scoped_refptr<history::ShortcutsBackend> shortcuts_backend_;

  
  
  ExitType last_session_exit_type_;

#if defined(ENABLE_SESSION_SERVICE)
  base::OneShotTimer<ProfileImpl> create_session_service_timer_;
#endif

  scoped_ptr<Profile> off_the_record_profile_;

  
  base::Time start_time_;

  scoped_refptr<history::TopSites> top_sites_;  

#if defined(OS_CHROMEOS)
  scoped_ptr<chromeos::Preferences> chromeos_preferences_;

  scoped_ptr<chromeos::LocaleChangeGuard> locale_change_guard_;

  bool is_login_profile_;
#endif

  scoped_ptr<PrefProxyConfigTracker> pref_proxy_config_tracker_;

  
  
  
  
  
  
  
  
  
  
  
  
  

  Profile::Delegate* delegate_;

  chrome_browser_net::Predictor* predictor_;

  DISALLOW_COPY_AND_ASSIGN(ProfileImpl);
};

#endif  
