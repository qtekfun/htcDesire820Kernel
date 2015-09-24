// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DRIVE_DRIVE_NOTIFICATION_MANAGER_H_
#define CHROME_BROWSER_DRIVE_DRIVE_NOTIFICATION_MANAGER_H_

#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/timer/timer.h"
#include "chrome/browser/drive/drive_notification_observer.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "sync/notifier/invalidation_handler.h"

class ProfileSyncService;

namespace invalidation {
class InvalidationService;
}

namespace drive {

class DriveNotificationManager
    : public BrowserContextKeyedService,
      public syncer::InvalidationHandler {
 public:
  explicit DriveNotificationManager(
      invalidation::InvalidationService* invalidation_service);
  virtual ~DriveNotificationManager();

  
  virtual void Shutdown() OVERRIDE;

  
  virtual void OnInvalidatorStateChange(
      syncer::InvalidatorState state) OVERRIDE;
  virtual void OnIncomingInvalidation(
      const syncer::ObjectIdInvalidationMap& invalidation_map) OVERRIDE;

  void AddObserver(DriveNotificationObserver* observer);
  void RemoveObserver(DriveNotificationObserver* observer);

  
  bool push_notification_enabled() const {
    return push_notification_enabled_;
  }

  
  bool push_notification_registered() const {
    return push_notification_registered_;
  }

 private:
  enum NotificationSource {
    NOTIFICATION_XMPP,
    NOTIFICATION_POLLING,
  };

  
  void RestartPollingTimer();

  
  
  void NotifyObserversToUpdate(NotificationSource source);

  
  void RegisterDriveNotifications();

  
  static std::string NotificationSourceToString(NotificationSource source);

  invalidation::InvalidationService* invalidation_service_;
  ObserverList<DriveNotificationObserver> observers_;

  
  bool push_notification_registered_;
  
  bool push_notification_enabled_;
  
  bool observers_notified_;

  
  
  base::Timer polling_timer_;

  
  
  base::WeakPtrFactory<DriveNotificationManager> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DriveNotificationManager);
};

}  

#endif  
