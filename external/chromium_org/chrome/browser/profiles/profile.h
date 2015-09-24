// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_PROFILES_PROFILE_H_
#define CHROME_BROWSER_PROFILES_PROFILE_H_

#include <string>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/logging.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/content_browser_client.h"
#include "net/url_request/url_request_job_factory.h"

class ChromeAppCacheService;
class ExtensionService;
class ExtensionSpecialStoragePolicy;
class FaviconService;
class HostContentSettingsMap;
class PasswordStore;
class PrefProxyConfigTracker;
class PrefService;
class PromoCounter;
class ProtocolHandlerRegistry;
class TestingProfile;
class WebDataService;

namespace android {
class TabContentsProvider;
}

namespace base {
class SequencedTaskRunner;
class Time;
}

namespace chrome_browser_net {
class Predictor;
}

namespace chromeos {
class LibCrosServiceLibraryImpl;
class ResetDefaultProxyConfigServiceTask;
}

namespace content {
class WebUI;
}

namespace fileapi {
class FileSystemContext;
}

namespace history {
class ShortcutsBackend;
class TopSites;
}

namespace net {
class SSLConfigService;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class Profile : public content::BrowserContext {
 public:
  
  
  
  
  
  
  
  
  
  enum ServiceAccessType {
    
    
    
    
    
    
    EXPLICIT_ACCESS,

    
    
    
    
    IMPLICIT_ACCESS
  };

  enum CreateStatus {
    
    CREATE_STATUS_LOCAL_FAIL,
    
    
    CREATE_STATUS_REMOTE_FAIL,
    
    CREATE_STATUS_CREATED,
    
    CREATE_STATUS_INITIALIZED,
    
    
    CREATE_STATUS_CANCELED,
    MAX_CREATE_STATUS  
  };

  enum CreateMode {
    CREATE_MODE_SYNCHRONOUS,
    CREATE_MODE_ASYNCHRONOUS
  };

  enum ExitType {
    
    
    EXIT_NORMAL,

    
    EXIT_SESSION_ENDED,

    EXIT_CRASHED,
  };

  class Delegate {
   public:
    virtual ~Delegate();

    
    virtual void OnProfileCreated(Profile* profile,
                                  bool success,
                                  bool is_new_profile) = 0;
  };

  
  static const char kProfileKey[];

  Profile();
  virtual ~Profile();

  
  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  
  static Profile* CreateProfile(const base::FilePath& path,
                                Delegate* delegate,
                                CreateMode create_mode);

  
  static Profile* FromBrowserContext(content::BrowserContext* browser_context);

  
  static Profile* FromWebUI(content::WebUI* web_ui);

  

  
  virtual TestingProfile* AsTestingProfile();

  
  
  virtual scoped_refptr<base::SequencedTaskRunner> GetIOTaskRunner() = 0;

  
  
  virtual std::string GetProfileName() = 0;

  
  
  
  
  
  
  
  virtual Profile* GetOffTheRecordProfile() = 0;

  
  virtual void DestroyOffTheRecordProfile() = 0;

  
  virtual bool HasOffTheRecordProfile() = 0;

  
  
  virtual Profile* GetOriginalProfile() = 0;

  
  virtual bool IsManaged() = 0;

  
  
  virtual history::TopSites* GetTopSites() = 0;

  
  virtual history::TopSites* GetTopSitesWithoutCreating() = 0;

  
  
  
  
  virtual ExtensionService* GetExtensionService() = 0;

  
  virtual ExtensionSpecialStoragePolicy*
      GetExtensionSpecialStoragePolicy() = 0;

  
  
  virtual PrefService* GetPrefs() = 0;

  
  
  
  virtual PrefService* GetOffTheRecordPrefs() = 0;

  
  virtual net::URLRequestContextGetter* GetRequestContext() = 0;

  
  
  virtual net::URLRequestContextGetter* GetRequestContextForExtensions() = 0;

  
  virtual net::SSLConfigService* GetSSLConfigService() = 0;

  
  virtual HostContentSettingsMap* GetHostContentSettingsMap() = 0;

  
  
  
  
  virtual bool IsSameProfile(Profile* profile) = 0;

  
  
  
  
  virtual base::Time GetStartTime() const = 0;

  
  
  
  
  
  
  
  virtual net::URLRequestContextGetter* CreateRequestContext(
      content::ProtocolHandlerMap* protocol_handlers) = 0;

  
  
  
  
  
  
  virtual net::URLRequestContextGetter* CreateRequestContextForStoragePartition(
      const base::FilePath& partition_path,
      bool in_memory,
      content::ProtocolHandlerMap* protocol_handlers) = 0;

  
  virtual base::FilePath last_selected_directory() = 0;
  virtual void set_last_selected_directory(const base::FilePath& path) = 0;

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

  
  virtual void InitChromeOSPreferences() = 0;
#endif  

  
  
  virtual PrefProxyConfigTracker* GetProxyConfigTracker() = 0;

  
  virtual chrome_browser_net::Predictor* GetNetworkPredictor() = 0;

  
  
  
  
  
  
  virtual void ClearNetworkingHistorySince(base::Time time,
                                           const base::Closure& completion) = 0;

  
  virtual GURL GetHomePage() = 0;

  
  
  virtual bool WasCreatedByVersionOrLater(const std::string& version) = 0;

  std::string GetDebugName();

  
  virtual bool IsGuestSession() const;

  
  void set_restored_last_session(bool restored_last_session) {
    restored_last_session_ = restored_last_session;
  }
  bool restored_last_session() const {
    return restored_last_session_;
  }

  
  
  // EXIT_CRASHED to one of the other values) is written to prefs, any
  
  
  
  
  
  virtual void SetExitType(ExitType exit_type) = 0;

  
  virtual ExitType GetLastSessionExitType() = 0;

  
  
  
  void PauseAccessibilityEvents() {
    accessibility_pause_level_++;
  }

  void ResumeAccessibilityEvents() {
    DCHECK_GT(accessibility_pause_level_, 0);
    accessibility_pause_level_--;
  }

  bool ShouldSendAccessibilityEvents() {
    return 0 == accessibility_pause_level_;
  }

  
  
  bool IsNewProfile();

  
  
  bool IsSyncAccessible();

  
  
  
  void MaybeSendDestroyedNotification();

  
  Profile* CreateOffTheRecordProfile();

 private:
  bool restored_last_session_;

  
  
  bool sent_destroyed_notification_;

  
  
  
  
  int accessibility_pause_level_;

  DISALLOW_COPY_AND_ASSIGN(Profile);
};

#if defined(COMPILER_GCC)
namespace BASE_HASH_NAMESPACE {

template<>
struct hash<Profile*> {
  std::size_t operator()(Profile* const& p) const {
    return reinterpret_cast<std::size_t>(p);
  }
};

}  
#endif

#endif  
