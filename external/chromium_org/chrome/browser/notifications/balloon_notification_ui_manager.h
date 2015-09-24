// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_BALLOON_NOTIFICATION_UI_MANAGER_H_
#define CHROME_BROWSER_NOTIFICATIONS_BALLOON_NOTIFICATION_UI_MANAGER_H_

#include <deque>
#include <string>

#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/notifications/balloon.h"
#include "chrome/browser/notifications/balloon_collection.h"
#include "chrome/browser/notifications/fullscreen_notification_blocker.h"
#include "chrome/browser/notifications/notification_prefs_manager.h"
#include "chrome/browser/notifications/notification_system_observer.h"
#include "chrome/browser/notifications/notification_ui_manager.h"
#include "chrome/browser/notifications/screen_lock_notification_blocker.h"

class Notification;
class PrefService;
class Profile;
class QueuedNotification;

class BalloonNotificationUIManager
    : public NotificationUIManager,
      public NotificationPrefsManager,
      public BalloonCollection::BalloonSpaceChangeListener,
      public message_center::NotificationBlocker::Observer {
 public:
  explicit BalloonNotificationUIManager(PrefService* local_state);
  virtual ~BalloonNotificationUIManager();

  
  
  void SetBalloonCollection(BalloonCollection* balloon_collection);

  
  virtual void Add(const Notification& notification,
                   Profile* profile) OVERRIDE;
  virtual bool Update(const Notification& notification,
                      Profile* profile) OVERRIDE;
  virtual const Notification* FindById(
      const std::string& notification_id) const OVERRIDE;
  virtual bool CancelById(const std::string& notification_id) OVERRIDE;
  virtual std::set<std::string> GetAllIdsByProfileAndSourceOrigin(
      Profile* profile,
      const GURL& source) OVERRIDE;
  virtual bool CancelAllBySourceOrigin(const GURL& source_origin) OVERRIDE;
  virtual bool CancelAllByProfile(Profile* profile) OVERRIDE;
  virtual void CancelAll() OVERRIDE;

  
  virtual BalloonCollection::PositionPreference
      GetPositionPreference() const OVERRIDE;
  virtual void SetPositionPreference(
      BalloonCollection::PositionPreference preference) OVERRIDE;

  BalloonCollection* balloon_collection();
  NotificationPrefsManager* prefs_manager();

  
  
  static BalloonNotificationUIManager* GetInstanceForTesting();

  void GetQueuedNotificationsForTesting(
    std::vector<const Notification*>* notifications);

 private:
  bool ShowNotification(const Notification& notification, Profile* profile);
  bool UpdateNotification(const Notification& notification, Profile* profile);

  
  
  
  void CheckAndShowNotifications();

  void ShowNotifications();

  void OnDesktopNotificationPositionChanged();

  
  virtual void OnBalloonSpaceChanged() OVERRIDE;

  
  virtual void OnBlockingStateChanged(
      message_center::NotificationBlocker* blocker) OVERRIDE;

  
  typedef std::deque<linked_ptr<QueuedNotification> > NotificationDeque;
  NotificationDeque show_queue_;

  
  scoped_ptr<BalloonCollection> balloon_collection_;

  ScreenLockNotificationBlocker screen_lock_blocker_;
  FullscreenNotificationBlocker fullscreen_blocker_;

  
  IntegerPrefMember position_pref_;

  NotificationSystemObserver system_observer_;

  DISALLOW_COPY_AND_ASSIGN(BalloonNotificationUIManager);
};

#endif  
