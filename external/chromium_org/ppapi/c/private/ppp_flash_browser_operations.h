/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPP_FLASH_BROWSER_OPERATIONS_H_
#define PPAPI_C_PRIVATE_PPP_FLASH_BROWSER_OPERATIONS_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"

#define PPP_FLASH_BROWSEROPERATIONS_INTERFACE_1_0 \
    "PPP_Flash_BrowserOperations;1.0"
#define PPP_FLASH_BROWSEROPERATIONS_INTERFACE_1_2 \
    "PPP_Flash_BrowserOperations;1.2"
#define PPP_FLASH_BROWSEROPERATIONS_INTERFACE_1_3 \
    "PPP_Flash_BrowserOperations;1.3"
#define PPP_FLASH_BROWSEROPERATIONS_INTERFACE \
    PPP_FLASH_BROWSEROPERATIONS_INTERFACE_1_3



typedef enum {
  PP_FLASH_BROWSEROPERATIONS_SETTINGTYPE_CAMERAMIC = 0,
  PP_FLASH_BROWSEROPERATIONS_SETTINGTYPE_PEERNETWORKING = 1
} PP_Flash_BrowserOperations_SettingType;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_Flash_BrowserOperations_SettingType, 4);

typedef enum {
  
  PP_FLASH_BROWSEROPERATIONS_PERMISSION_DEFAULT = 0,
  PP_FLASH_BROWSEROPERATIONS_PERMISSION_ALLOW = 1,
  PP_FLASH_BROWSEROPERATIONS_PERMISSION_BLOCK = 2,
  PP_FLASH_BROWSEROPERATIONS_PERMISSION_ASK = 3
} PP_Flash_BrowserOperations_Permission;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_Flash_BrowserOperations_Permission, 4);

struct PP_Flash_BrowserOperations_SiteSetting {
  const char* site;
  PP_Flash_BrowserOperations_Permission permission;
};

typedef void (*PPB_Flash_BrowserOperations_GetSettingsCallback)(
    void* user_data,
    PP_Bool success,
    PP_Flash_BrowserOperations_Permission default_permission,
    uint32_t site_count,
    const struct PP_Flash_BrowserOperations_SiteSetting sites[]);

struct PPP_Flash_BrowserOperations_1_3 {
  PP_Bool (*ClearSiteData)(const char* plugin_data_path,
                           const char* site,
                           uint64_t flags,
                           uint64_t max_age);
  /**
   * Requests the plugin to deauthorize content licenses. It prevents Flash from
   * playing protected content, such as movies and music the user may have
   * rented or purchased.
   *
   * @param[in] plugin_data_path String containing the directory where the
   * plugin settings are stored.
   *
   * @return <code>PP_TRUE</code> on success, <code>PP_FALSE</code> on failure.
   */
  PP_Bool (*DeauthorizeContentLicenses)(const char* plugin_data_path);
  void (*GetPermissionSettings)(
      const char* plugin_data_path,
      PP_Flash_BrowserOperations_SettingType setting_type,
      PPB_Flash_BrowserOperations_GetSettingsCallback callback,
      void* user_data);
  PP_Bool (*SetDefaultPermission)(
      const char* plugin_data_path,
      PP_Flash_BrowserOperations_SettingType setting_type,
      PP_Flash_BrowserOperations_Permission permission,
      PP_Bool clear_site_specific);
  PP_Bool (*SetSitePermission)(
      const char* plugin_data_path,
      PP_Flash_BrowserOperations_SettingType setting_type,
      uint32_t site_count,
      const struct PP_Flash_BrowserOperations_SiteSetting sites[]);
  void (*GetSitesWithData)(const char* plugin_data_path, char** sites[]);
  void (*FreeSiteList)(char* sites[]);
};

typedef struct PPP_Flash_BrowserOperations_1_3 PPP_Flash_BrowserOperations;

struct PPP_Flash_BrowserOperations_1_0 {
  PP_Bool (*ClearSiteData)(const char* plugin_data_path,
                           const char* site,
                           uint64_t flags,
                           uint64_t max_age);
};

struct PPP_Flash_BrowserOperations_1_2 {
  PP_Bool (*ClearSiteData)(const char* plugin_data_path,
                           const char* site,
                           uint64_t flags,
                           uint64_t max_age);
  PP_Bool (*DeauthorizeContentLicenses)(const char* plugin_data_path);
  void (*GetPermissionSettings)(
      const char* plugin_data_path,
      PP_Flash_BrowserOperations_SettingType setting_type,
      PPB_Flash_BrowserOperations_GetSettingsCallback callback,
      void* user_data);
  PP_Bool (*SetDefaultPermission)(
      const char* plugin_data_path,
      PP_Flash_BrowserOperations_SettingType setting_type,
      PP_Flash_BrowserOperations_Permission permission,
      PP_Bool clear_site_specific);
  PP_Bool (*SetSitePermission)(
      const char* plugin_data_path,
      PP_Flash_BrowserOperations_SettingType setting_type,
      uint32_t site_count,
      const struct PP_Flash_BrowserOperations_SiteSetting sites[]);
};

#endif  

