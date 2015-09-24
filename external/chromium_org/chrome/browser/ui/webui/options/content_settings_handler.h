// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CONTENT_SETTINGS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CONTENT_SETTINGS_HANDLER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/pepper_flash_settings_manager.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "chrome/browser/ui/webui/options/pepper_flash_content_settings_utils.h"
#include "chrome/common/content_settings.h"
#include "chrome/common/content_settings_types.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class HostContentSettingsMap;
class ProtocolHandlerRegistry;

namespace options {

class ContentSettingsHandler : public OptionsPageUIHandler,
                               public PepperFlashSettingsManager::Client {
 public:
  ContentSettingsHandler();
  virtual ~ContentSettingsHandler();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;
  virtual void InitializeHandler() OVERRIDE;
  virtual void InitializePage() OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnGetPermissionSettingsCompleted(
      uint32 request_id,
      bool success,
      PP_Flash_BrowserOperations_Permission default_permission,
      const ppapi::FlashSiteSettings& sites) OVERRIDE;

  
  static std::string ContentSettingsTypeToGroupName(ContentSettingsType type);

 private:
  
  
  struct MediaSettingsInfo {
    MediaSettingsInfo();
    ~MediaSettingsInfo();

    
    ContentSetting flash_default_setting;
    MediaExceptions flash_exceptions;
    bool flash_settings_initialized;
    uint32_t last_flash_refresh_request_id;

    
    bool show_flash_default_link;
    bool show_flash_exceptions_link;

    
    ContentSetting default_setting;
    bool policy_disable_audio;
    bool policy_disable_video;
    bool default_setting_initialized;
    MediaExceptions exceptions;
    bool exceptions_initialized;
  };

  
  enum LinkType {
    DEFAULT_SETTING = 0,
    EXCEPTIONS,
  };

  

  
  void UpdateSettingDefaultFromModel(ContentSettingsType type);

  
  void UpdateMediaSettingsView();

  
  void UpdateExceptionsViewFromModel(ContentSettingsType type);
  
  
  void UpdateOTRExceptionsViewFromModel(ContentSettingsType type);
  
  
  void UpdateAllExceptionsViewsFromModel();
  
  void UpdateAllOTRExceptionsViewsFromModel();
  
  void UpdateGeolocationExceptionsView();
  
  void UpdateNotificationExceptionsView();
  
  void UpdateMediaExceptionsView();
  
  void UpdateMIDISysExExceptionsView();
  
  
  void UpdateExceptionsViewFromHostContentSettingsMap(ContentSettingsType type);
  
  void UpdateExceptionsViewFromOTRHostContentSettingsMap(
      ContentSettingsType type);
  
  void UpdateHandlersEnabledRadios();
  
  void RemoveGeolocationException(const base::ListValue* args,
                                  size_t arg_index);
  
  void RemoveNotificationException(const base::ListValue* args,
                                   size_t arg_index);
  
  void RemoveMediaException(const base::ListValue* args, size_t arg_index);
  
  void RemoveExceptionFromHostContentSettingsMap(
      const base::ListValue* args,
      size_t arg_index,
      ContentSettingsType type);

  

  
  
  
  void SetContentFilter(const base::ListValue* args);

  
  
  
  void RemoveException(const base::ListValue* args);

  
  
  void SetException(const base::ListValue* args);

  
  
  void CheckExceptionPatternValidity(const base::ListValue* args);

  

  
  void ApplyWhitelist(ContentSettingsType content_type,
                      ContentSetting default_setting);

  
  HostContentSettingsMap* GetContentSettingsMap();

  
  
  HostContentSettingsMap* GetOTRContentSettingsMap();

  
  
  std::string GetSettingDefaultFromModel(ContentSettingsType type,
                                         std::string* provider_id);

  
  ProtocolHandlerRegistry* GetProtocolHandlerRegistry();

  void RefreshFlashMediaSettings();

  
  void GetExceptionsFromHostContentSettingsMap(
      const HostContentSettingsMap* map,
      ContentSettingsType type,
      base::ListValue* exceptions);

  void OnPepperFlashPrefChanged();

  void ShowFlashMediaLink(LinkType link_type, bool show);

  void UpdateFlashMediaLinksVisibility();

  void UpdateProtectedContentExceptionsButton();

  

  content::NotificationRegistrar notification_registrar_;
  PrefChangeRegistrar pref_change_registrar_;
  scoped_ptr<PepperFlashSettingsManager> flash_settings_manager_;
  MediaSettingsInfo media_settings_;

  DISALLOW_COPY_AND_ASSIGN(ContentSettingsHandler);
};

}  

#endif  
