// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_DESKTOP_NOTIFICATION_SERVICE_H_
#define CHROME_BROWSER_NOTIFICATIONS_DESKTOP_NOTIFICATION_SERVICE_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "chrome/browser/content_settings/content_settings_notification_provider.h"
#include "chrome/browser/content_settings/content_settings_provider.h"
#include "chrome/browser/prefs/pref_change_registrar.h"
#include "chrome/browser/profiles/profile_keyed_service.h"
#include "chrome/common/content_settings.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "googleurl/src/gurl.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebTextDirection.h"

class Notification;
class NotificationUIManager;
class NotificationsPrefsCache;
class PrefService;
class Profile;
class TabContents;
struct DesktopNotificationHostMsg_Show_Params;

class DesktopNotificationService : public NotificationObserver,
                                   public ProfileKeyedService {
 public:
  enum DesktopNotificationSource {
    PageNotification,
    WorkerNotification
  };

  DesktopNotificationService(Profile* profile,
                             NotificationUIManager* ui_manager);
  virtual ~DesktopNotificationService();

  
  
  
  
  
  void RequestPermission(const GURL& origin,
                         int process_id,
                         int route_id,
                         int callback_context,
                         TabContents* tab);

  
  
  
  
  bool ShowDesktopNotification(
      const DesktopNotificationHostMsg_Show_Params& params,
      int process_id, int route_id, DesktopNotificationSource source);

  
  
  
  bool CancelDesktopNotification(int process_id,
                                 int route_id,
                                 int notification_id);

  
  void GrantPermission(const GURL& origin);
  void DenyPermission(const GURL& origin);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  NotificationsPrefsCache* prefs_cache() { return prefs_cache_; }

  
  
  
  static string16 CreateDataUrl(const GURL& icon_url,
                                const string16& title,
                                const string16& body,
                                WebKit::WebTextDirection dir);

  
  
  
  static string16 CreateDataUrl(int resource,
                                const std::vector<std::string>& subst);

  
  
  ContentSetting GetDefaultContentSetting();
  void SetDefaultContentSetting(ContentSetting setting);
  bool IsDefaultContentSettingManaged() const;

  
  void ResetToDefaultContentSetting();

  
  std::vector<GURL> GetAllowedOrigins();

  
  std::vector<GURL> GetBlockedOrigins();

  
  void ResetAllowedOrigin(const GURL& origin);

  
  void ResetBlockedOrigin(const GURL& origin);

  
  void ResetAllOrigins();

  static void RegisterUserPrefs(PrefService* user_prefs);

  ContentSetting GetContentSetting(const GURL& origin);

 private:
  void InitPrefs();
  void StartObserving();
  void StopObserving();

  void OnPrefsChanged(const std::string& pref_name);

  
  void ShowNotification(const Notification& notification);

  
  
  
  string16 DisplayNameForOrigin(const GURL& origin);

  
  void NotifySettingsChange();

  
  Profile* profile_;

  
  
  scoped_refptr<NotificationsPrefsCache> prefs_cache_;

  
  
  NotificationUIManager* ui_manager_;

  scoped_ptr<content_settings::NotificationProvider> provider_;

  PrefChangeRegistrar prefs_registrar_;
  NotificationRegistrar notification_registrar_;

  DISALLOW_COPY_AND_ASSIGN(DesktopNotificationService);
};

#endif  
