// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CONTENT_SETTINGS_PERMISSION_QUEUE_CONTROLLER_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_PERMISSION_QUEUE_CONTROLLER_H_

#include "base/bind.h"
#include "chrome/common/content_settings_types.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class GURL;
class PermissionRequestID;
class InfoBarService;
class Profile;

class PermissionQueueController : public content::NotificationObserver {
 public:
  typedef base::Callback<void(bool )> PermissionDecidedCallback;

  PermissionQueueController(Profile* profile, ContentSettingsType type);
  virtual ~PermissionQueueController();

  
  
  void CreateInfoBarRequest(const PermissionRequestID& id,
                            const GURL& requesting_frame,
                            const GURL& embedder,
                            PermissionDecidedCallback callback);

  
  void CancelInfoBarRequest(const PermissionRequestID& id);

  
  
  
  void OnPermissionSet(const PermissionRequestID& id,
                       const GURL& requesting_frame,
                       const GURL& embedder,
                       bool update_content_setting,
                       bool allowed);

 protected:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  class PendingInfoBarRequest;
  class RequestEquals;

  typedef std::vector<PendingInfoBarRequest> PendingInfoBarRequests;

  
  
  bool AlreadyShowingInfoBarForTab(const PermissionRequestID& id) const;

  
  
  
  
  
  void ShowQueuedInfoBarForTab(const PermissionRequestID& id);

  void ClearPendingInfoBarRequestsForTab(const PermissionRequestID& id);

  void RegisterForInfoBarNotifications(InfoBarService* infobar_service);
  void UnregisterForInfoBarNotifications(InfoBarService* infobar_service);

  void UpdateContentSetting(
      const GURL& requesting_frame, const GURL& embedder, bool allowed);

  content::NotificationRegistrar registrar_;

  Profile* const profile_;
  ContentSettingsType type_;
  PendingInfoBarRequests pending_infobar_requests_;
  bool in_shutdown_;

  DISALLOW_COPY_AND_ASSIGN(PermissionQueueController);
};

#endif  
