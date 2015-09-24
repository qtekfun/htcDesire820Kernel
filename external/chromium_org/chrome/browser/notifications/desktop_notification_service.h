// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_DESKTOP_NOTIFICATION_SERVICE_H_
#define CHROME_BROWSER_NOTIFICATIONS_DESKTOP_NOTIFICATION_SERVICE_H_

#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_member.h"
#include "base/strings/string16.h"
#include "chrome/browser/content_settings/content_settings_provider.h"
#include "chrome/browser/notifications/welcome_notification.h"
#include "chrome/common/content_settings.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "third_party/WebKit/public/web/WebNotificationPresenter.h"
#include "third_party/WebKit/public/web/WebTextDirection.h"
#include "ui/message_center/notifier_settings.h"
#include "url/gurl.h"

class ContentSettingsPattern;
class Notification;
class NotificationDelegate;
class NotificationUIManager;
class Profile;

namespace content {
class WebContents;
struct ShowDesktopNotificationHostMsgParams;
}

namespace gfx {
class Image;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class DesktopNotificationService : public BrowserContextKeyedService,
                                   public content::NotificationObserver {
 public:
  enum DesktopNotificationSource {
    PageNotification,
    WorkerNotification
  };

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* prefs);

  DesktopNotificationService(Profile* profile,
                             NotificationUIManager* ui_manager);
  virtual ~DesktopNotificationService();

  
  
  
  void RequestPermission(const GURL& origin,
                         int process_id,
                         int route_id,
                         int callback_context,
                         content::WebContents* tab);

  
  
  
  
  bool ShowDesktopNotification(
      const content::ShowDesktopNotificationHostMsgParams& params,
      int process_id,
      int route_id,
      DesktopNotificationSource source);

  
  
  
  bool CancelDesktopNotification(int process_id,
                                 int route_id,
                                 int notification_id);

  
  void GrantPermission(const GURL& origin);
  void DenyPermission(const GURL& origin);

  
  
  
  static base::string16 CreateDataUrl(const GURL& icon_url,
                                      const base::string16& title,
                                      const base::string16& body,
                                      blink::WebTextDirection dir);

  
  
  
  static base::string16 CreateDataUrl(int resource,
                                const std::vector<std::string>& subst);

  
  
  
  
  
  static std::string AddNotification(const GURL& origin_url,
                                     const base::string16& title,
                                     const base::string16& message,
                                     const GURL& icon_url,
                                     const base::string16& replace_id,
                                     NotificationDelegate* delegate,
                                     Profile* profile);

  
  static std::string AddIconNotification(const GURL& origin_url,
                                         const base::string16& title,
                                         const base::string16& message,
                                         const gfx::Image& icon,
                                         const base::string16& replace_id,
                                         NotificationDelegate* delegate,
                                         Profile* profile);

  
  static void RemoveNotification(const std::string& notification_id);

  
  
  
  ContentSetting GetDefaultContentSetting(std::string* provider_id);
  void SetDefaultContentSetting(ContentSetting setting);

  
  void ResetToDefaultContentSetting();

  
  
  void GetNotificationsSettings(ContentSettingsForOneType* settings);

  
  void ClearSetting(const ContentSettingsPattern& pattern);

  
  void ResetAllOrigins();

  ContentSetting GetContentSetting(const GURL& origin);

  
  
  bool IsNotifierEnabled(const message_center::NotifierId& notifier_id);

  
  void SetNotifierEnabled(const message_center::NotifierId& notifier_id,
                          bool enabled);

  
  
  void ShowWelcomeNotificationIfNecessary(const Notification& notification);

 private:
  
  void ShowNotification(const Notification& notification);

  
  
  
  base::string16 DisplayNameForOriginInProcessId(const GURL& origin,
                                                 int process_id);

  
  void NotifySettingsChange();

  NotificationUIManager* GetUIManager();

  
  void OnStringListPrefChanged(
      const char* pref_name, std::set<std::string>* ids_field);

  
  void OnDisabledExtensionIdsChanged();

  
  void OnDisabledSystemComponentIdsChanged();

  
  void OnEnabledSyncNotifierIdsChanged();

  void FirePermissionLevelChangedEvent(
      const message_center::NotifierId& notifier_id,
      bool enabled);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  Profile* profile_;

  
  
  NotificationUIManager* ui_manager_;

  
  StringListPrefMember disabled_extension_id_pref_;

  
  StringListPrefMember disabled_system_component_id_pref_;

  
  StringListPrefMember enabled_sync_notifier_id_pref_;

  
  std::set<std::string> disabled_extension_ids_;

  
  std::set<std::string> disabled_system_component_ids_;

  
  std::set<std::string> enabled_sync_notifier_ids_;

  
  content::NotificationRegistrar registrar_;

  
  scoped_ptr<WelcomeNotification> welcome_notification;

  DISALLOW_COPY_AND_ASSIGN(DesktopNotificationService);
};

#endif  
