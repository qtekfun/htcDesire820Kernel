// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_GOOGLE_UPDATE_SETTINGS_H_
#define CHROME_INSTALLER_UTIL_GOOGLE_UPDATE_SETTINGS_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "base/version.h"
#include "chrome/installer/util/util_constants.h"

class BrowserDistribution;

namespace installer {
class ChannelInfo;
class InstallationState;
}

class GoogleUpdateSettings {
 public:
  
  enum UpdatePolicy {
    UPDATES_DISABLED    = 0,
    AUTOMATIC_UPDATES   = 1,
    MANUAL_UPDATES_ONLY = 2,
    AUTO_UPDATES_ONLY   = 3,
    UPDATE_POLICIES_COUNT
  };

  
  struct ProductData {
    
    std::string version;
    
    
    
    base::Time last_success;
    
    int last_result;
    
    
    int last_error_code;
    
    
    
    int last_extra_code;
  };

  
  static bool IsSystemInstall();

  
  
  
  static bool GetCollectStatsConsent();

  
  
  static bool SetCollectStatsConsent(bool consented);

#if defined(OS_WIN)
  
  
  
  static bool GetCollectStatsConsentAtLevel(bool system_install);

  
  
  static bool SetCollectStatsConsentAtLevel(bool system_install,
                                            bool consented);
#endif

  
  
  static bool GetMetricsId(std::string* metrics_id);

  
  static bool SetMetricsId(const std::string& metrics_id);

  
  
  static bool SetEULAConsent(const installer::InstallationState& machine_state,
                             BrowserDistribution* dist,
                             bool consented);

  
  
  
  static int GetLastRunTime();

  
  
  
  static bool SetLastRunTime();

  
  
  
  static bool RemoveLastRunTime();

  
  
  static bool GetBrowser(std::wstring* browser);

  
  
  
  static bool GetLanguage(std::wstring* language);

  
  
  
  
  
  static bool GetBrand(std::wstring* brand);

  
  
  
  
  
  
  
  static bool GetReactivationBrand(std::wstring* brand);

  
  
  static bool GetClient(std::wstring* client);

  
  
  static bool SetClient(const std::wstring& client);

  
  
  static bool GetReferral(std::wstring* referral);

  
  
  static bool ClearReferral();

  
  
  static bool UpdateDidRunState(bool did_run, bool system_level);

  
  
  static bool UpdateDidRunStateForDistribution(BrowserDistribution* dist,
                                               bool did_run,
                                               bool system_level);

  
  
  
  static std::wstring GetChromeChannel(bool system_install);

  
  
  
  
  
  static bool GetChromeChannelAndModifiers(bool system_install,
                                           string16* channel);

  
  
  
  
  
  
  
  
  
  
  static void UpdateInstallStatus(bool system_install,
                                  installer::ArchiveType archive_type,
                                  int install_return_code,
                                  const std::wstring& product_guid);

  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool UpdateGoogleUpdateApKey(installer::ArchiveType archive_type,
                                      int install_return_code,
                                      installer::ChannelInfo* value);

  
  
  static void UpdateProfileCounts(int profiles_active, int profiles_signedin);

  
  
  
  
  
  
  
  
  
  static int DuplicateGoogleUpdateSystemClientKey();

  
  
  static bool WriteGoogleUpdateSystemClientKey(int handle,
                                               const std::wstring& key,
                                               const std::wstring& value);

  
  
  
  static UpdatePolicy GetAppUpdatePolicy(const std::wstring& app_guid,
                                         bool* is_overridden);

  
  static void RecordChromeUpdatePolicyHistograms();

  
  
  static string16 GetUninstallCommandLine(bool system_install);

  
  static Version GetGoogleUpdateVersion(bool system_install);

  
  
  static base::Time GetGoogleUpdateLastStartedAU(bool system_install);

  
  
  
  static base::Time GetGoogleUpdateLastChecked(bool system_install);

  
  
  
  static bool GetUpdateDetailForApp(bool system_install,
                                    const wchar_t* app_guid,
                                    ProductData* data);

  
  
  static bool GetUpdateDetailForGoogleUpdate(bool system_install,
                                             ProductData* data);

  
  
  static bool GetUpdateDetail(bool system_install, ProductData* data);

  
  
  
  
  
  
  static bool SetExperimentLabels(bool system_install,
                                  const string16& experiment_labels);

  
  
  
  
  
  
  static bool ReadExperimentLabels(bool system_install,
                                   string16* experiment_labels);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(GoogleUpdateSettings);
};

#endif  
