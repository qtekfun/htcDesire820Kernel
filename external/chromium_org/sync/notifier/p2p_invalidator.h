// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_NOTIFIER_P2P_INVALIDATOR_H_
#define SYNC_NOTIFIER_P2P_INVALIDATOR_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/threading/thread_checker.h"
#include "jingle/notifier/base/notifier_options.h"
#include "jingle/notifier/listener/push_client.h"
#include "jingle/notifier/listener/push_client_observer.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/notifier/invalidator.h"
#include "sync/notifier/invalidator_registrar.h"
#include "sync/notifier/invalidator_state.h"
#include "sync/notifier/object_id_invalidation_map.h"

namespace notifier {
class PushClient;
}  

namespace syncer {

SYNC_EXPORT extern const char kSyncP2PNotificationChannel[];

enum P2PNotificationTarget {
  NOTIFY_SELF,
  FIRST_NOTIFICATION_TARGET = NOTIFY_SELF,
  NOTIFY_OTHERS,
  NOTIFY_ALL,
  LAST_NOTIFICATION_TARGET = NOTIFY_ALL
};

SYNC_EXPORT_PRIVATE std::string P2PNotificationTargetToString(
    P2PNotificationTarget target);

SYNC_EXPORT_PRIVATE P2PNotificationTarget P2PNotificationTargetFromString(
    const std::string& target_str);

class SYNC_EXPORT_PRIVATE P2PNotificationData {
 public:
  
  
  P2PNotificationData();
  P2PNotificationData(const std::string& sender_id,
                      P2PNotificationTarget target,
                      const ObjectIdInvalidationMap& invalidation_map);

  ~P2PNotificationData();

  
  bool IsTargeted(const std::string& id) const;

  const ObjectIdInvalidationMap& GetIdInvalidationMap() const;

  bool Equals(const P2PNotificationData& other) const;

  std::string ToString() const;

  
  
  bool ResetFromString(const std::string& str);

 private:
  
  std::string sender_id_;
  
  P2PNotificationTarget target_;
  
  ObjectIdInvalidationMap invalidation_map_;
};

class SYNC_EXPORT_PRIVATE P2PInvalidator
    : public Invalidator,
      public NON_EXPORTED_BASE(notifier::PushClientObserver) {
 public:
  
  
  
  
  
  P2PInvalidator(scoped_ptr<notifier::PushClient> push_client,
                 const std::string& invalidator_client_id,
                 P2PNotificationTarget send_notification_target);

  virtual ~P2PInvalidator();

  
  virtual void RegisterHandler(InvalidationHandler* handler) OVERRIDE;
  virtual void UpdateRegisteredIds(InvalidationHandler* handler,
                                   const ObjectIdSet& ids) OVERRIDE;
  virtual void UnregisterHandler(InvalidationHandler* handler) OVERRIDE;
  virtual InvalidatorState GetInvalidatorState() const OVERRIDE;
  virtual void UpdateCredentials(
      const std::string& email, const std::string& token) OVERRIDE;

  
  virtual void OnNotificationsEnabled() OVERRIDE;
  virtual void OnNotificationsDisabled(
      notifier::NotificationsDisabledReason reason) OVERRIDE;
  virtual void OnIncomingNotification(
      const notifier::Notification& notification) OVERRIDE;

  void SendInvalidation(const ObjectIdSet& ids);

  void SendNotificationDataForTest(
      const P2PNotificationData& notification_data);

 private:
  void SendNotificationData(const P2PNotificationData& notification_data);

  base::ThreadChecker thread_checker_;

  InvalidatorRegistrar registrar_;

  
  scoped_ptr<notifier::PushClient> push_client_;
  
  std::string invalidator_client_id_;
  
  bool logged_in_;
  bool notifications_enabled_;
  
  P2PNotificationTarget send_notification_target_;

  DISALLOW_COPY_AND_ASSIGN(P2PInvalidator);
};

}  

#endif  
