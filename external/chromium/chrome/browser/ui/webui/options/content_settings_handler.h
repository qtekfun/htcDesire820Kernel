// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CONTENT_SETTINGS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CONTENT_SETTINGS_HANDLER_H_
#pragma once

#include "chrome/browser/plugin_data_remover_helper.h"
#include "chrome/browser/prefs/pref_change_registrar.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "chrome/common/content_settings_types.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class HostContentSettingsMap;

class ContentSettingsHandler : public OptionsPageUIHandler {
 public:
  ContentSettingsHandler();
  virtual ~ContentSettingsHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);

  virtual void Initialize();

  virtual void RegisterMessages();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  static std::string ContentSettingsTypeToGroupName(ContentSettingsType type);

 private:
  

  
  void UpdateSettingDefaultFromModel(ContentSettingsType type);

  
  void UpdateExceptionsViewFromModel(ContentSettingsType type);
  
  
  void UpdateOTRExceptionsViewFromModel(ContentSettingsType type);
  
  
  void UpdateAllExceptionsViewsFromModel();
  
  void UpdateAllOTRExceptionsViewsFromModel();
  
  void UpdateGeolocationExceptionsView();
  
  void UpdateNotificationExceptionsView();
  
  
  void UpdateExceptionsViewFromHostContentSettingsMap(ContentSettingsType type);
  
  void UpdateExceptionsViewFromOTRHostContentSettingsMap(
      ContentSettingsType type);

  

  
  
  
  void SetContentFilter(const ListValue* args);

  
  
  
  void RemoveException(const ListValue* args);

  
  
  void SetException(const ListValue* args);

  
  
  void CheckExceptionPatternValidity(const ListValue* args);

  
  void SetAllowThirdPartyCookies(const ListValue* args);

  

  
  HostContentSettingsMap* GetContentSettingsMap();

  
  
  HostContentSettingsMap* GetOTRContentSettingsMap();

  
  std::string GetSettingDefaultFromModel(ContentSettingsType type);

  
  
  bool GetDefaultSettingManagedFromModel(ContentSettingsType type);

  

  NotificationRegistrar notification_registrar_;
  PrefChangeRegistrar pref_change_registrar_;

  DISALLOW_COPY_AND_ASSIGN(ContentSettingsHandler);
};

#endif  
