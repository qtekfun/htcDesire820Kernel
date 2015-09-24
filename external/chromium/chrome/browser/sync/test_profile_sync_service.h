// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_PROFILE_SYNC_SERVICE_H_
#define CHROME_BROWSER_SYNC_TEST_PROFILE_SYNC_SERVICE_H_
#pragma once

#include <string>

#include "chrome/browser/sync/glue/data_type_manager_impl.h"
#include "chrome/browser/sync/js_backend.h"
#include "chrome/browser/sync/profile_sync_service.h"
#include "chrome/test/profile_mock.h"
#include "chrome/test/sync/engine/test_id_factory.h"
#include "testing/gmock/include/gmock/gmock.h"

class Profile;
class Task;
class TestProfileSyncService;

ACTION(ReturnNewDataTypeManager) {
  return new browser_sync::DataTypeManagerImpl(arg0, arg1);
}

namespace browser_sync {

class SyncBackendHostForProfileSyncTest
    : public SyncBackendHost, public JsBackend {
 public:
  
  
  SyncBackendHostForProfileSyncTest(
      Profile* profile,
      bool set_initial_sync_ended_on_init,
      bool synchronous_init);
  virtual ~SyncBackendHostForProfileSyncTest();

  MOCK_METHOD1(RequestNudge, void(const tracked_objects::Location&));

  virtual void ConfigureDataTypes(
      const DataTypeController::TypeMap& data_type_controllers,
      const syncable::ModelTypeSet& types,
      CancelableTask* ready_task);

  
  void SimulateSyncCycleCompletedInitialSyncEnded(
      const tracked_objects::Location&);

  virtual sync_api::HttpPostProviderFactory* MakeHttpBridgeFactory(
      net::URLRequestContextGetter* getter);

  virtual void InitCore(const Core::DoInitializeOptions& options);

  virtual JsBackend* GetJsBackend();

  
  virtual void SetParentJsEventRouter(JsEventRouter* router);
  virtual void RemoveParentJsEventRouter();
  virtual const JsEventRouter* GetParentJsEventRouter() const;
  
  
  
  virtual void ProcessMessage(const std::string& name, const JsArgList& args,
                              const JsEventHandler* sender);

  virtual void StartConfiguration(Callback0::Type* callback);

  static void SetDefaultExpectationsForWorkerCreation(ProfileMock* profile);

  static void SetHistoryServiceExpectations(ProfileMock* profile);

 private:
  bool synchronous_init_;
};

}  

class TestProfileSyncService : public ProfileSyncService {
 public:
  
  
  TestProfileSyncService(ProfileSyncFactory* factory,
                         Profile* profile,
                         const std::string& test_user,
                         bool synchronous_backend_initialization,
                         Task* initial_condition_setup_task);

  virtual ~TestProfileSyncService();

  void SetInitialSyncEndedForEnabledTypes();

  virtual void OnBackendInitialized();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  void dont_set_initial_sync_ended_on_init();
  void set_synchronous_sync_configuration();

  browser_sync::TestIdFactory* id_factory();

  
  
  
  virtual browser_sync::SyncBackendHostForProfileSyncTest*
      GetBackendForTest();

 protected:
  virtual void CreateBackend();

 private:
  
  
  virtual std::string GetLsidForAuthBootstraping();

  browser_sync::TestIdFactory id_factory_;

  bool synchronous_backend_initialization_;

  
  
  bool synchronous_sync_configuration_;
  bool set_expect_resume_expectations_;

  Task* initial_condition_setup_task_;
  bool set_initial_sync_ended_on_init_;
};



#endif  
