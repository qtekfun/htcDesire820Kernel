// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_SYNC_NOTIFIER_CHROME_NOTIFIER_SERVICE_H_
#define CHROME_BROWSER_NOTIFICATIONS_SYNC_NOTIFIER_CHROME_NOTIFIER_SERVICE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_vector.h"
#include "base/prefs/pref_member.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/notifications/sync_notifier/synced_notification.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "sync/api/syncable_service.h"

class NotificationUIManager;
class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace message_center {
struct Notifier;
}

namespace notifier {

extern const char kFirstSyncedNotificationServiceId[];
extern const char kServiceEnabledOnce[];
extern const char kSyncedNotificationFirstRun[];

enum ChromeNotifierServiceActionType {
  CHROME_NOTIFIER_SERVICE_ACTION_UNKNOWN,
  CHROME_NOTIFIER_SERVICE_ACTION_FIRST_SERVICE_ENABLED,
  CHROME_NOTIFIER_SERVICE_ACTION_FIRST_SERVICE_DISABLED,
  
  
  
  CHROME_NOTIFIER_SERVICE_ACTION_COUNT
};

class ChromeNotifierService : public syncer::SyncableService,
                              public BrowserContextKeyedService {
 public:
  ChromeNotifierService(Profile* profile, NotificationUIManager* manager);
  virtual ~ChromeNotifierService();

  
  virtual void Shutdown() OVERRIDE;

  
  virtual syncer::SyncMergeResult MergeDataAndStartSyncing(
      syncer::ModelType type,
      const syncer::SyncDataList& initial_sync_data,
      scoped_ptr<syncer::SyncChangeProcessor> sync_processor,
      scoped_ptr<syncer::SyncErrorFactory> error_handler) OVERRIDE;
  virtual void StopSyncing(syncer::ModelType type) OVERRIDE;
  virtual syncer::SyncDataList GetAllSyncData(
      syncer::ModelType type) const OVERRIDE;
  virtual syncer::SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const syncer::SyncChangeList& change_list) OVERRIDE;

  
  static syncer::SyncData CreateSyncDataFromNotification(
      const SyncedNotification& notification);

  
  static scoped_ptr<SyncedNotification> CreateNotificationFromSyncData(
      const syncer::SyncData& sync_data);

  
  virtual notifier::SyncedNotification* FindNotificationById(
      const std::string& notification_id);

  
  
  void GetSyncedNotificationServices(
      std::vector<message_center::Notifier*>* notifiers);

  
  
  virtual void MarkNotificationAsRead(const std::string& id);

  
  void OnSyncedNotificationServiceEnabled(
      const std::string& notifier_id,
      bool enabled);

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  Profile* profile() const { return profile_; }

  
  void AddForTest(scoped_ptr<notifier::SyncedNotification> notification);

  
  
  
  
  static void set_avoid_bitmap_fetching_for_test(bool avoid) {
    avoid_bitmap_fetching_for_test_ = avoid;
  }

  
  void InitializePrefs();

 private:
  
  void Add(scoped_ptr<notifier::SyncedNotification> notification);

  
  void UpdateInMessageCenter(notifier::SyncedNotification* notification);

  
  void Display(notifier::SyncedNotification* notification);

  
  void FreeNotificationById(const std::string& notification_id);

  
  
  void DisplayUnreadNotificationsFromSource(const std::string& notifier_id);

  
  
  void RemoveUnreadNotificationsFromSource(const std::string& notifier_id);

  
  
  void CollectPerServiceEnablingStatistics(const std::string& notifier_id,
                                           bool enabled);

  
  void AddNewSendingServices();

  
  void OnEnabledSendingServiceListPrefChanged(std::set<std::string>* ids_field);

  
  void OnInitializedSendingServiceListPrefChanged(
      std::set<std::string>* ids_field);

  
  void OnSyncedNotificationFirstRunBooleanPrefChanged(bool* new_value);

  
  
  void BuildServiceListValueInplace(
      std::set<std::string> services, base::ListValue* list_value);

  
  StringListPrefMember enabled_sending_services_prefs_;
  StringListPrefMember initialized_sending_services_prefs_;

  
  BooleanPrefMember synced_notification_first_run_prefs_;

  
  Profile* const profile_;
  NotificationUIManager* const notification_manager_;
  scoped_ptr<syncer::SyncChangeProcessor> sync_processor_;
  std::set<std::string> enabled_sending_services_;
  std::set<std::string> initialized_sending_services_;
  bool synced_notification_first_run_;
  static bool avoid_bitmap_fetching_for_test_;

  
  
  ScopedVector<notifier::SyncedNotification> notification_data_;

  friend class ChromeNotifierServiceTest;
  FRIEND_TEST_ALL_PREFIXES(ChromeNotifierServiceTest, ServiceEnabledTest);
  FRIEND_TEST_ALL_PREFIXES(ChromeNotifierServiceTest,
                           AddNewSendingServicesTest);
  FRIEND_TEST_ALL_PREFIXES(ChromeNotifierServiceTest,
                           GetEnabledSendingServicesFromPreferencesTest);


  DISALLOW_COPY_AND_ASSIGN(ChromeNotifierService);
};

}  

#endif  
