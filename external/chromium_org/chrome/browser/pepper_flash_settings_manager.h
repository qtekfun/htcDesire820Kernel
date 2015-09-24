// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PEPPER_FLASH_SETTINGS_MANAGER_H_
#define CHROME_BROWSER_PEPPER_FLASH_SETTINGS_MANAGER_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "ppapi/c/private/ppp_flash_browser_operations.h"
#include "ppapi/shared_impl/ppp_flash_browser_operations_shared.h"

class PluginPrefs;
class PrefService;

namespace content {
class BrowserContext;
struct WebPluginInfo;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class PepperFlashSettingsManager {
 public:
  class Client {
   public:
    virtual ~Client() {}

    virtual void OnDeauthorizeContentLicensesCompleted(uint32 request_id,
                                                       bool success) {}
    virtual void OnGetPermissionSettingsCompleted(
        uint32 request_id,
        bool success,
        PP_Flash_BrowserOperations_Permission default_permission,
        const ppapi::FlashSiteSettings& sites) {}

    virtual void OnSetDefaultPermissionCompleted(uint32 request_id,
                                                 bool success) {}

    virtual void OnSetSitePermissionCompleted(uint32 request_id,
                                              bool success) {}

    virtual void OnGetSitesWithDataCompleted(
        uint32 request_id,
        const std::vector<std::string>& sites) {}

    virtual void OnClearSiteDataCompleted(uint32 request_id, bool success) {}
  };

  
  
  PepperFlashSettingsManager(Client* client,
                             content::BrowserContext* browser_context);
  ~PepperFlashSettingsManager();

  
  
  static bool IsPepperFlashInUse(PluginPrefs* plugin_prefs,
                                 content::WebPluginInfo* plugin_info);

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  // Requests to deauthorize content licenses.
  // Client::OnDeauthorizeContentLicensesCompleted() will be called when the
  
  
  // Client::OnDeauthorizeContentLicensesCompleted().
  uint32 DeauthorizeContentLicenses(PrefService* prefs);

  
  
  
  uint32 GetPermissionSettings(
      PP_Flash_BrowserOperations_SettingType setting_type);

  
  
  
  uint32 SetDefaultPermission(
      PP_Flash_BrowserOperations_SettingType setting_type,
      PP_Flash_BrowserOperations_Permission permission,
      bool clear_site_specific);

  
  
  
  uint32 SetSitePermission(PP_Flash_BrowserOperations_SettingType setting_type,
                           const ppapi::FlashSiteSettings& sites);

  
  
  
  uint32 GetSitesWithData();

  
  
  
  uint32 ClearSiteData(const std::string& site, uint64 flags, uint64 max_age);

 private:
  
  
  
  
  
  
  class Core;

  uint32 GetNextRequestId();

  void EnsureCoreExists();

  
  void OnError(Core* core);

  
  Client* client_;

  
  content::BrowserContext* browser_context_;

  scoped_refptr<Core> core_;

  uint32 next_request_id_;

  base::WeakPtrFactory<PepperFlashSettingsManager> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PepperFlashSettingsManager);
};

#endif  
