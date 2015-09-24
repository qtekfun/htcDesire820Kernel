// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_TESTING_PROFILE_H_
#define CHROME_TEST_BASE_TESTING_PROFILE_H_

#include <string>

#include "base/files/scoped_temp_dir.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/profiles/profile.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

namespace content {
class MockResourceContext;
}

namespace extensions {
class ExtensionPrefs;
}

namespace history {
class TopSites;
}

namespace net {
class CookieMonster;
class URLRequestContextGetter;
}

namespace policy {
class PolicyService;
class ProfilePolicyConnector;
class SchemaRegistryService;
}

namespace quota {
class SpecialStoragePolicy;
}

class BrowserContextDependencyManager;
class CommandLine;
class ExtensionSpecialStoragePolicy;
class HostContentSettingsMap;
class PrefServiceSyncable;
class ProfileSyncService;
class TemplateURLService;
class TestingPrefServiceSyncable;

class TestingProfile : public Profile {
 public:
  
  
  
  
  
  
  static const char kTestUserProfileDir[];

  
  TestingProfile();

  typedef std::vector<std::pair<
              BrowserContextKeyedServiceFactory*,
              BrowserContextKeyedServiceFactory::TestingFactoryFunction> >
      TestingFactories;

  
  
  
  
  class Builder {
   public:
    Builder();
    ~Builder();

    
    
    
    
    
    void SetDelegate(Delegate* delegate);

    
    
    void AddTestingFactory(
        BrowserContextKeyedServiceFactory* service_factory,
        BrowserContextKeyedServiceFactory::TestingFactoryFunction callback);

    
    
    void SetExtensionSpecialStoragePolicy(
        scoped_refptr<ExtensionSpecialStoragePolicy> policy);

    
    void SetPath(const base::FilePath& path);

    
    void SetPrefService(scoped_ptr<PrefServiceSyncable> prefs);

    
    void SetIncognito();

    
    void SetGuestSession();

    
    
    void SetManagedUserId(const std::string& managed_user_id);

    
    void SetPolicyService(scoped_ptr<policy::PolicyService> policy_service);

    
    scoped_ptr<TestingProfile> Build();

   private:
    
    bool build_called_;

    
    scoped_ptr<PrefServiceSyncable> pref_service_;
    scoped_refptr<ExtensionSpecialStoragePolicy> extension_policy_;
    base::FilePath path_;
    Delegate* delegate_;
    bool incognito_;
    bool guest_session_;
    std::string managed_user_id_;
    scoped_ptr<policy::PolicyService> policy_service_;
    TestingFactories testing_factories_;

    DISALLOW_COPY_AND_ASSIGN(Builder);
  };

  
  
  
  
  
  
  explicit TestingProfile(const base::FilePath& path);

  
  
  
  TestingProfile(const base::FilePath& path, Delegate* delegate);

  
  
  TestingProfile(const base::FilePath& path,
                 Delegate* delegate,
                 scoped_refptr<ExtensionSpecialStoragePolicy> extension_policy,
                 scoped_ptr<PrefServiceSyncable> prefs,
                 bool incognito,
                 bool guest_session,
                 const std::string& managed_user_id,
                 scoped_ptr<policy::PolicyService> policy_service,
                 const TestingFactories& factories);

  virtual ~TestingProfile();

  
  void CreateFaviconService();

  
  
  
  
  
  
  bool CreateHistoryService(bool delete_file, bool no_db) WARN_UNUSED_RESULT;

  
  void DestroyHistoryService();

  
  
  
  void CreateTopSites();

  
  void DestroyTopSites();

  
  
  
  
  
  
  
  
  void CreateBookmarkModel(bool delete_file);

  
  void CreateWebDataService();

  
  
  void BlockUntilHistoryIndexIsRefreshed();

  
  void BlockUntilTopSitesLoaded();

  TestingPrefServiceSyncable* GetTestingPrefService();

  
  virtual base::FilePath GetPath() const OVERRIDE;
  virtual scoped_refptr<base::SequencedTaskRunner> GetIOTaskRunner() OVERRIDE;
  virtual bool IsOffTheRecord() const OVERRIDE;
  virtual content::DownloadManagerDelegate*
      GetDownloadManagerDelegate() OVERRIDE;
  virtual net::URLRequestContextGetter* GetRequestContext() OVERRIDE;
  virtual net::URLRequestContextGetter* CreateRequestContext(
      content::ProtocolHandlerMap* protocol_handlers) OVERRIDE;
  virtual net::URLRequestContextGetter* GetRequestContextForRenderProcess(
      int renderer_child_id) OVERRIDE;
  virtual content::ResourceContext* GetResourceContext() OVERRIDE;
  virtual content::GeolocationPermissionContext*
      GetGeolocationPermissionContext() OVERRIDE;
  virtual quota::SpecialStoragePolicy* GetSpecialStoragePolicy() OVERRIDE;

  virtual TestingProfile* AsTestingProfile() OVERRIDE;
  virtual std::string GetProfileName() OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  
  
  void ForceIncognito(bool force_incognito) {
    force_incognito_ = force_incognito;
  }

  
  virtual void SetOffTheRecordProfile(scoped_ptr<Profile> profile);
  virtual void SetOriginalProfile(Profile* profile);
  virtual Profile* GetOffTheRecordProfile() OVERRIDE;
  virtual void DestroyOffTheRecordProfile() OVERRIDE {}
  virtual bool HasOffTheRecordProfile() OVERRIDE;
  virtual Profile* GetOriginalProfile() OVERRIDE;
  virtual bool IsManaged() OVERRIDE;
  virtual ExtensionService* GetExtensionService() OVERRIDE;
  void SetExtensionSpecialStoragePolicy(
      ExtensionSpecialStoragePolicy* extension_special_storage_policy);
  virtual ExtensionSpecialStoragePolicy*
      GetExtensionSpecialStoragePolicy() OVERRIDE;
  
  
  
  net::CookieMonster* GetCookieMonster();

  virtual PrefService* GetPrefs() OVERRIDE;

  virtual history::TopSites* GetTopSites() OVERRIDE;
  virtual history::TopSites* GetTopSitesWithoutCreating() OVERRIDE;

  virtual net::URLRequestContextGetter* GetMediaRequestContext() OVERRIDE;
  virtual net::URLRequestContextGetter* GetMediaRequestContextForRenderProcess(
      int renderer_child_id) OVERRIDE;
  virtual net::URLRequestContextGetter*
      GetRequestContextForExtensions() OVERRIDE;
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
  virtual net::URLRequestContextGetter* CreateRequestContextForStoragePartition(
      const base::FilePath& partition_path,
      bool in_memory,
      content::ProtocolHandlerMap* protocol_handlers) OVERRIDE;
  virtual net::SSLConfigService* GetSSLConfigService() OVERRIDE;
  virtual HostContentSettingsMap* GetHostContentSettingsMap() OVERRIDE;
  virtual std::wstring GetName();
  virtual void SetName(const std::wstring& name) {}
  virtual std::wstring GetID();
  virtual void SetID(const std::wstring& id);
  void set_last_session_exited_cleanly(bool value) {
    last_session_exited_cleanly_ = value;
  }
  virtual void MergeResourceString(int message_id,
                                   std::wstring* output_string) {}
  virtual void MergeResourceInteger(int message_id, int* output_value) {}
  virtual void MergeResourceBoolean(int message_id, bool* output_value) {}
  virtual bool IsSameProfile(Profile *p) OVERRIDE;
  virtual base::Time GetStartTime() const OVERRIDE;
  virtual base::FilePath last_selected_directory() OVERRIDE;
  virtual void set_last_selected_directory(const base::FilePath& path) OVERRIDE;
  virtual bool WasCreatedByVersionOrLater(const std::string& version) OVERRIDE;
  virtual bool IsGuestSession() const OVERRIDE;
  virtual void SetExitType(ExitType exit_type) OVERRIDE {}
  virtual ExitType GetLastSessionExitType() OVERRIDE;
#if defined(OS_CHROMEOS)
  virtual void ChangeAppLocale(const std::string&,
                               AppLocaleChangedVia) OVERRIDE {
  }
  virtual void OnLogin() OVERRIDE {
  }
  virtual void InitChromeOSPreferences() OVERRIDE {
  }
#endif  

  virtual PrefProxyConfigTracker* GetProxyConfigTracker() OVERRIDE;

  
  
  
  void BlockUntilHistoryProcessesPendingRequests();

  virtual chrome_browser_net::Predictor* GetNetworkPredictor() OVERRIDE;
  virtual void ClearNetworkingHistorySince(
      base::Time time,
      const base::Closure& completion) OVERRIDE;
  virtual GURL GetHomePage() OVERRIDE;

  virtual PrefService* GetOffTheRecordPrefs() OVERRIDE;

  void set_profile_name(const std::string& profile_name) {
    profile_name_ = profile_name;
  }

 protected:
  base::Time start_time_;
  scoped_ptr<PrefServiceSyncable> prefs_;
  
  TestingPrefServiceSyncable* testing_prefs_;

 private:
  
  void CreateTempProfileDir();

  
  void Init();

  
  void FinishInit();

  
  void CreateTestingPrefService();

  
  
  void CreateProfilePolicyConnector();

  
  
  scoped_refptr<net::URLRequestContextGetter> extensions_request_context_;

  std::wstring id_;

  bool incognito_;
  bool force_incognito_;
  scoped_ptr<Profile> incognito_profile_;
  Profile* original_profile_;

  bool guest_session_;

  std::string managed_user_id_;

  
  bool last_session_exited_cleanly_;

  scoped_refptr<HostContentSettingsMap> host_content_settings_map_;

  base::FilePath last_selected_directory_;
  scoped_refptr<history::TopSites> top_sites_;  

  scoped_refptr<ExtensionSpecialStoragePolicy>
      extension_special_storage_policy_;

  
  scoped_ptr<PrefProxyConfigTracker> pref_proxy_config_tracker_;

  
  
  
  base::ScopedTempDir temp_dir_;
  
  
  base::FilePath profile_path_;

  
  
  
  BrowserContextDependencyManager* browser_context_dependency_manager_;

  
  
  content::MockResourceContext* resource_context_;

#if defined(ENABLE_CONFIGURATION_POLICY)
  scoped_ptr<policy::SchemaRegistryService> schema_registry_service_;
#endif
  scoped_ptr<policy::ProfilePolicyConnector> profile_policy_connector_;

  
  Delegate* delegate_;

  std::string profile_name_;

  scoped_ptr<policy::PolicyService> policy_service_;
};

#endif  
