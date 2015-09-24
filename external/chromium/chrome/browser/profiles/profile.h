// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_PROFILES_PROFILE_H_
#define CHROME_BROWSER_PROFILES_PROFILE_H_
#pragma once

#ifdef ANDROID
#include "base/base_api.h"
#endif
#include "base/basictypes.h"
#include "base/logging.h"
#ifndef ANDROID
#include "chrome/common/extensions/extension.h"
#endif

namespace base {
class Time;
}

namespace content {
class ResourceContext;
}

namespace fileapi {
class FileSystemContext;
class SandboxedFileSystemContext;
}

namespace history {
class TopSites;
}

namespace net {
class TransportSecurityState;
class SSLConfigService;
}

namespace policy {
class ProfilePolicyConnector;
}

namespace prerender {
class PrerenderManager;
}

namespace webkit_database {
class DatabaseTracker;
}

#ifdef ANDROID
struct UnloadedExtensionInfo {
  enum Reason {
    DISABLE,    
    UPDATE,     
    UNINSTALL,  
  };
};
#endif

class AutocompleteClassifier;
class BookmarkModel;
class BrowserSignin;
class ChromeAppCacheService;
class ChromeBlobStorageContext;
class ChromeURLDataManager;
class CloudPrintProxyService;
class DownloadManager;
class Extension;
class ExtensionDevToolsManager;
class ExtensionEventRouter;
class ExtensionInfoMap;
class ExtensionMessageService;
class ExtensionPrefValueMap;
class ExtensionProcessManager;
class ExtensionService;
class ExtensionSpecialStoragePolicy;
class FaviconService;
class FilePath;
class FindBarState;
class GeolocationContentSettingsMap;
class GeolocationPermissionContext;
class HistoryService;
class HostContentSettingsMap;
class HostZoomMap;
class NTPResourceCache;
class NavigationController;
class PasswordStore;
class PersonalDataManager;
class PrefProxyConfigTracker;
class PrefService;
class ProfileSyncFactory;
class ProfileSyncService;
class PromoCounter;
class ProtocolHandlerRegistry;
class SQLitePersistentCookieStore;
class SSLConfigServiceManager;
class SSLHostState;
class SessionService;
class SpellCheckHost;
class StatusTray;
class TabRestoreService;
class TemplateURLFetcher;
class TemplateURLModel;
class TokenService;
class TransportSecurityPersister;
class UserScriptMaster;
class UserStyleSheetWatcher;
class VisitedLinkEventListener;
class VisitedLinkMaster;
class WebDataService;
class WebKitContext;
class PromoResourceService;

namespace net {
class URLRequestContextGetter;
}

typedef intptr_t ProfileId;

class
#ifdef ANDROID
BASE_API
#endif
Profile {
 public:
  
  
  
  
  
  
  
  
  
  enum ServiceAccessType {
    
    
    
    
    
    
    EXPLICIT_ACCESS,

    
    
    
    
    IMPLICIT_ACCESS
  };

  class Delegate {
   public:
    
    virtual void OnProfileCreated(Profile* profile, bool success) = 0;
#ifdef ANDROID
    virtual ~Delegate() {}
#endif
  };

  
  static const char* kProfileKey;

  
  static const ProfileId kInvalidProfileId;

  Profile();
  virtual ~Profile() {}

  
  
  static void RegisterUserPrefs(PrefService* prefs);

  
  static Profile* CreateProfile(const FilePath& path);

  
  static Profile* CreateProfileAsync(const FilePath& path,
                                     Delegate* delegate);

  
  
  
  
  static net::URLRequestContextGetter* GetDefaultRequestContext();

  
  
  virtual ProfileId GetRuntimeId() = 0;

  
  virtual FilePath GetPath() = 0;

  
  virtual bool IsOffTheRecord() = 0;

  
  
  
  virtual Profile* GetOffTheRecordProfile() = 0;

  
  virtual void DestroyOffTheRecordProfile() = 0;

  
  virtual bool HasOffTheRecordProfile() = 0;

  
  
  virtual Profile* GetOriginalProfile() = 0;

  
  virtual ChromeAppCacheService* GetAppCacheService() = 0;

  
  virtual webkit_database::DatabaseTracker* GetDatabaseTracker() = 0;

  
  
  virtual history::TopSites* GetTopSites() = 0;

  
  virtual history::TopSites* GetTopSitesWithoutCreating() = 0;

  
  
  
  virtual VisitedLinkMaster* GetVisitedLinkMaster() = 0;

  
  
  virtual ExtensionService* GetExtensionService() = 0;

  
  
  
  virtual UserScriptMaster* GetUserScriptMaster() = 0;

  
  
  virtual ExtensionDevToolsManager* GetExtensionDevToolsManager() = 0;

  
  
  virtual ExtensionProcessManager* GetExtensionProcessManager() = 0;

  
  
  virtual ExtensionMessageService* GetExtensionMessageService() = 0;

  
  virtual ExtensionEventRouter* GetExtensionEventRouter() = 0;

  
  virtual ExtensionSpecialStoragePolicy*
      GetExtensionSpecialStoragePolicy() = 0;

  
  
  
  virtual SSLHostState* GetSSLHostState() = 0;

  
  
  
  virtual net::TransportSecurityState* GetTransportSecurityState() = 0;

  
  
  
  
  
  
  
  
  
  
  
  virtual FaviconService* GetFaviconService(ServiceAccessType access) = 0;

  
  
  
  
  
  
  
  
  
  
  
  virtual HistoryService* GetHistoryService(ServiceAccessType access) = 0;

  
  
  virtual HistoryService* GetHistoryServiceWithoutCreating() = 0;

  
  
  
  virtual AutocompleteClassifier* GetAutocompleteClassifier() = 0;

  
  
  
  
  
  
  virtual WebDataService* GetWebDataService(ServiceAccessType access) = 0;

  
  
  virtual WebDataService* GetWebDataServiceWithoutCreating() = 0;

  
  virtual PasswordStore* GetPasswordStore(ServiceAccessType access) = 0;

  
  
  
  virtual PrefService* GetPrefs() = 0;

  
  
  
  virtual PrefService* GetOffTheRecordPrefs() = 0;

  
  
  virtual TemplateURLModel* GetTemplateURLModel() = 0;

  
  
  virtual TemplateURLFetcher* GetTemplateURLFetcher() = 0;

  
  virtual DownloadManager* GetDownloadManager() = 0;
  virtual bool HasCreatedDownloadManager() const = 0;

  
  virtual PersonalDataManager* GetPersonalDataManager() = 0;

  
  
  
  virtual fileapi::FileSystemContext* GetFileSystemContext() = 0;

  
  virtual BrowserSignin* GetBrowserSignin() = 0;

  
  
  
  virtual net::URLRequestContextGetter* GetRequestContext() = 0;

  
  
  
  
  
  virtual net::URLRequestContextGetter* GetRequestContextForPossibleApp(
      const Extension* installed_app) = 0;

  
  
  virtual net::URLRequestContextGetter* GetRequestContextForMedia() = 0;

  
  
  virtual net::URLRequestContextGetter* GetRequestContextForExtensions() = 0;

  
  
  virtual net::URLRequestContextGetter* GetRequestContextForIsolatedApp(
      const std::string& app_id) = 0;

#ifndef ANDROID
  virtual const content::ResourceContext& GetResourceContext() = 0;
#endif

  
  
  
  
  
  virtual void RegisterExtensionWithRequestContexts(
      const Extension* extension) {}

  
  
  
  virtual void UnregisterExtensionWithRequestContexts(
      const std::string& extension_id,
      const UnloadedExtensionInfo::Reason) {}

  
  virtual net::SSLConfigService* GetSSLConfigService() = 0;

  
  virtual HostContentSettingsMap* GetHostContentSettingsMap() = 0;

  
  virtual HostZoomMap* GetHostZoomMap() = 0;

  
  virtual GeolocationContentSettingsMap* GetGeolocationContentSettingsMap() = 0;

  
  virtual GeolocationPermissionContext* GetGeolocationPermissionContext() = 0;

  
  virtual UserStyleSheetWatcher* GetUserStyleSheetWatcher() = 0;

  
  
  virtual FindBarState* GetFindBarState() = 0;

  
  
  
  
  
  
  
  
  virtual SessionService* GetSessionService() = 0;

  
  
  
  virtual void ShutdownSessionService() = 0;

  
  virtual bool HasSessionService() const = 0;

  
  virtual bool HasProfileSyncService() const = 0;

  
  virtual bool DidLastSessionExitCleanly() = 0;

  
  virtual BookmarkModel* GetBookmarkModel() = 0;

  
  virtual ProtocolHandlerRegistry* GetProtocolHandlerRegistry() = 0;

  
  virtual TokenService* GetTokenService() = 0;

  
  virtual ProfileSyncService* GetProfileSyncService() = 0;

  
  
  virtual ProfileSyncService* GetProfileSyncService(
      const std::string& cros_user) = 0;

  
  virtual CloudPrintProxyService* GetCloudPrintProxyService() = 0;

  
  
  
  
  virtual bool IsSameProfile(Profile* profile) = 0;

  
  
  
  
  virtual base::Time GetStartTime() const = 0;

  
  virtual TabRestoreService* GetTabRestoreService() = 0;

  virtual void ResetTabRestoreService() = 0;

  
  virtual SpellCheckHost* GetSpellCheckHost() = 0;

  
  
  
  virtual void ReinitializeSpellCheckHost(bool force) = 0;

  
  virtual WebKitContext* GetWebKitContext() = 0;

  
  
  
  virtual StatusTray* GetStatusTray() = 0;

  
  
  
  
  virtual void MarkAsCleanShutdown() = 0;

  
  
  
  virtual void InitExtensions(bool extensions_enabled) = 0;

  
  virtual void InitPromoResources() = 0;

  
  
  virtual void InitRegisteredProtocolHandlers() = 0;

  
  virtual NTPResourceCache* GetNTPResourceCache() = 0;

  
  virtual FilePath last_selected_directory() = 0;
  virtual void set_last_selected_directory(const FilePath& path) = 0;

  
  
  virtual ChromeBlobStorageContext* GetBlobStorageContext() = 0;

  
  virtual ExtensionInfoMap* GetExtensionInfoMap() = 0;

  
  virtual PromoCounter* GetInstantPromoCounter() = 0;

  
  virtual policy::ProfilePolicyConnector* GetPolicyConnector() = 0;

  
  virtual ChromeURLDataManager* GetChromeURLDataManager() = 0;

#if defined(OS_CHROMEOS)
  enum AppLocaleChangedVia {
    
    APP_LOCALE_CHANGED_VIA_SETTINGS,
    
    APP_LOCALE_CHANGED_VIA_REVERT,
    
    APP_LOCALE_CHANGED_VIA_LOGIN,
    
    APP_LOCALE_CHANGED_VIA_UNKNOWN
  };

  
  virtual void ChangeAppLocale(
      const std::string& locale, AppLocaleChangedVia via) = 0;

  
  virtual void OnLogin() = 0;

  
  virtual void SetupChromeOSEnterpriseExtensionObserver() = 0;

  
  virtual void InitChromeOSPreferences() = 0;
#endif  

  
  
  virtual PrefProxyConfigTracker* GetProxyConfigTracker() = 0;

  
  
  virtual prerender::PrerenderManager* GetPrerenderManager() = 0;

  
  static bool IsGuestSession();

#ifdef UNIT_TEST
  
  static void set_default_request_context(net::URLRequestContextGetter* c) {
    default_request_context_ = c;
  }
#endif

  
  void set_restored_last_session(bool restored_last_session) {
    restored_last_session_ = restored_last_session;
  }
  bool restored_last_session() const {
    return restored_last_session_;
  }

  
  
  
  void PauseAccessibilityEvents() {
    accessibility_pause_level_++;
  }

  void ResumeAccessibilityEvents() {
    DCHECK(accessibility_pause_level_ > 0);
    accessibility_pause_level_--;
  }

  bool ShouldSendAccessibilityEvents() {
    return 0 == accessibility_pause_level_;
  }

  
  
  bool IsSyncAccessible();

  
  Profile* CreateOffTheRecordProfile();

 protected:
  friend class OffTheRecordProfileImpl;

  static net::URLRequestContextGetter* default_request_context_;

 private:
  bool restored_last_session_;

  
  
  
  
  int accessibility_pause_level_;
};

#endif  
