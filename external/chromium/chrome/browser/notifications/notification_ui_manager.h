// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_UI_MANAGER_H_
#define CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_UI_MANAGER_H_
#pragma once

#include <deque>
#include <string>

#include "base/id_map.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer.h"
#include "chrome/browser/notifications/balloon.h"
#include "chrome/browser/notifications/balloon_collection.h"
#include "chrome/browser/prefs/pref_member.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class Notification;
class PrefService;
class Profile;
class QueuedNotification;
class SiteInstance;

class NotificationUIManager
    : public BalloonCollection::BalloonSpaceChangeListener,
      public NotificationObserver {
 public:
  explicit NotificationUIManager(PrefService* local_state);
  virtual ~NotificationUIManager();

  
  
  
  static NotificationUIManager* Create(PrefService* local_state);

  
  static void RegisterPrefs(PrefService* prefs);

  
  
  void Initialize(BalloonCollection* balloon_collection);

  
  virtual void Add(const Notification& notification,
                   Profile* profile);

  
  
  virtual bool CancelById(const std::string& notification_id);

  
  
  
  virtual bool CancelAllBySourceOrigin(const GURL& source_origin);

  
  
  void CancelAll();

  
  BalloonCollection* balloon_collection() {
    return balloon_collection_.get();
  }

  
  BalloonCollection::PositionPreference GetPositionPreference();

  
  
  void SetPositionPreference(BalloonCollection::PositionPreference preference);

 private:
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  void CheckAndShowNotifications();

  
  void ShowNotifications();

  
  virtual void OnBalloonSpaceChanged();

  
  
  bool TryReplacement(const Notification& notification);

  
  void CheckUserState();

  
  scoped_ptr<BalloonCollection> balloon_collection_;

  
  typedef std::deque<QueuedNotification*> NotificationDeque;
  NotificationDeque show_queue_;

  
  NotificationRegistrar registrar_;

  
  IntegerPrefMember position_pref_;

  
  bool is_user_active_;
  base::RepeatingTimer<NotificationUIManager> user_state_check_timer_;

  DISALLOW_COPY_AND_ASSIGN(NotificationUIManager);
};

#endif  
