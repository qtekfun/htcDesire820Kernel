// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_UI_MANAGER_MAC_H_
#define CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_UI_MANAGER_MAC_H_

#import <AppKit/AppKit.h>

#include <map>
#include <set>

#include "base/basictypes.h"
#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/notifications/balloon_notification_ui_manager.h"

@protocol CrUserNotification;
class Notification;
@class NotificationCenterDelegate;
class PrefService;
class Profile;

class NotificationUIManagerMac : public BalloonNotificationUIManager {
 public:
  explicit NotificationUIManagerMac(PrefService* local_state);
  virtual ~NotificationUIManagerMac();

  
  virtual void Add(const Notification& notification,
                   Profile* profile) OVERRIDE;
  virtual std::set<std::string> GetAllIdsByProfileAndSourceOrigin(
      Profile* profile, const GURL& source_origin) OVERRIDE;
  virtual bool CancelById(const std::string& notification_id) OVERRIDE;
  virtual bool CancelAllBySourceOrigin(const GURL& source_origin) OVERRIDE;
  virtual bool CancelAllByProfile(Profile* profile) OVERRIDE;
  virtual void CancelAll() OVERRIDE;

  
  
  const Notification* FindNotificationWithCocoaNotification(
      id<CrUserNotification> notification) const;

 private:
  
  
  
  struct ControllerNotification {
    ControllerNotification(Profile* a_profile,
                           id<CrUserNotification> a_view,
                           Notification* a_model);
    ~ControllerNotification();

    Profile* profile;
    id<CrUserNotification> view;
    Notification* model;

   private:
    DISALLOW_COPY_AND_ASSIGN(ControllerNotification);
  };
  typedef std::map<std::string, ControllerNotification*> NotificationMap;

  
  
  bool RemoveNotification(id<CrUserNotification> notification);

  
  id<CrUserNotification> FindNotificationWithReplacementId(
      const base::string16& replacement_id) const;

  
  base::scoped_nsobject<NotificationCenterDelegate> delegate_;

  
  
  
  NotificationMap notification_map_;

  DISALLOW_COPY_AND_ASSIGN(NotificationUIManagerMac);
};

#endif  
