// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_PERSONAL_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_PERSONAL_OPTIONS_HANDLER_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/browser_signin.h"
#include "chrome/browser/sync/profile_sync_service.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#if defined(OS_CHROMEOS)
#include "content/common/notification_registrar.h"
#endif

class OptionsManagedBannerHandler;

class PersonalOptionsHandler : public OptionsPageUIHandler,
                               public ProfileSyncServiceObserver,
                               public BrowserSignin::SigninDelegate {
 public:
  PersonalOptionsHandler();
  virtual ~PersonalOptionsHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);
  virtual void Initialize();

  
  virtual void RegisterMessages();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void OnStateChanged();

  
  virtual void OnLoginSuccess();
  virtual void OnLoginFailure(const GoogleServiceAuthError& error);

 private:
  void ObserveThemeChanged();
  void ShowSyncActionDialog(const ListValue* args);
  void ShowSyncLoginDialog(const ListValue* args);
  void ShowCustomizeSyncDialog(const ListValue* args);
  void ThemesReset(const ListValue* args);
#if defined(TOOLKIT_GTK)
  void ThemesSetGTK(const ListValue* args);
#endif

  
  
  void OnPreferredDataTypesUpdated(const ListValue* args);

#if defined(OS_CHROMEOS)
  void LoadAccountPicture(const ListValue* args);
  NotificationRegistrar registrar_;
#endif

  scoped_ptr<OptionsManagedBannerHandler> banner_handler_;

  
  bool multiprofile_;

  DISALLOW_COPY_AND_ASSIGN(PersonalOptionsHandler);
};

#endif  
