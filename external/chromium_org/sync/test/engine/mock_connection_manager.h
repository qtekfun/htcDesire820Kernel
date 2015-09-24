// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_TEST_ENGINE_MOCK_CONNECTION_MANAGER_H_
#define SYNC_TEST_ENGINE_MOCK_CONNECTION_MANAGER_H_

#include <bitset>
#include <list>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_vector.h"
#include "base/synchronization/lock.h"
#include "sync/engine/net/server_connection_manager.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/base/unique_position.h"
#include "sync/protocol/sync.pb.h"

namespace syncer {

class MockConnectionManager : public ServerConnectionManager {
 public:
  class MidCommitObserver {
   public:
    virtual void Observe() = 0;

   protected:
    virtual ~MidCommitObserver() {}
  };

  MockConnectionManager(syncable::Directory*,
                        CancelationSignal* signal);
  virtual ~MockConnectionManager();

  
  virtual bool PostBufferToPath(
      PostBufferParams*,
      const std::string& path,
      const std::string& auth_token,
      ScopedServerStatusWatcher* watcher) OVERRIDE;

  
  
  void SetMidCommitCallback(const base::Closure& callback);
  void SetMidCommitObserver(MidCommitObserver* observer);

  
  
  void SetCommitTimeRename(std::string prepend);

  
  
  
  
  
  sync_pb::SyncEntity* AddUpdateDirectory(
      syncable::Id id,
      syncable::Id parent_id,
      std::string name,
      int64 version,
      int64 sync_ts,
      std::string originator_cache_guid,
      std::string originator_client_item_id);
  sync_pb::SyncEntity* AddUpdateBookmark(syncable::Id id,
                                         syncable::Id parent_id,
                                         std::string name,
                                         int64 version,
                                         int64 sync_ts,
                                         std::string originator_cache_guid,
                                         std::string originator_client_item_id);
  
  sync_pb::SyncEntity* AddUpdateDirectory(
      int id,
      int parent_id,
      std::string name,
      int64 version,
      int64 sync_ts,
      std::string originator_cache_guid,
      std::string originator_client_item_id);
  sync_pb::SyncEntity* AddUpdateBookmark(int id,
                                         int parent_id,
                                         std::string name,
                                         int64 version,
                                         int64 sync_ts,
                                         std::string originator_cache_guid,
                                         std::string originator_client_item_id);
  
  sync_pb::SyncEntity* AddUpdateDirectory(
      std::string id,
      std::string parent_id,
      std::string name,
      int64 version,
      int64 sync_ts,
      std::string originator_cache_guid,
      std::string originator_client_item_id);
  sync_pb::SyncEntity* AddUpdateBookmark(std::string id,
                                         std::string parent_id,
                                         std::string name,
                                         int64 version,
                                         int64 sync_ts,
                                         std::string originator_cache_guid,
                                         std::string originator_client_item_id);
  
  sync_pb::SyncEntity* AddUpdateSpecifics(
      int id,
      int parent_id,
      std::string name,
      int64 version,
      int64 sync_ts,
      bool is_dir,
      int64 position,
      const sync_pb::EntitySpecifics& specifics);
  sync_pb::SyncEntity* AddUpdateSpecifics(
      int id,
      int parent_id,
      std::string name,
      int64 version,
      int64 sync_ts,
      bool is_dir,
      int64 position,
      const sync_pb::EntitySpecifics& specifics,
      std::string originator_cache_guid,
      std::string originator_client_item_id);
  sync_pb::SyncEntity* SetNigori(
      int id,
      int64 version,
      int64 sync_ts,
      const sync_pb::EntitySpecifics& specifics);
  
  sync_pb::SyncEntity* AddUpdatePref(std::string id,
                                     std::string parent_id,
                                     std::string client_tag,
                                     int64 version,
                                     int64 sync_ts);

  
  
  
  sync_pb::SyncEntity* AddUpdateFromLastCommit();

  
  
  
  void AddUpdateTombstone(const syncable::Id& id);

  void SetLastUpdateDeleted();
  void SetLastUpdateServerTag(const std::string& tag);
  void SetLastUpdateClientTag(const std::string& tag);
  void SetLastUpdateOriginatorFields(const std::string& client_id,
                                     const std::string& entry_id);
  void SetLastUpdatePosition(int64 position_in_parent);
  void SetNewTimestamp(int ts);
  void SetChangesRemaining(int64 count);

  
  
  
  void NextUpdateBatch();

  void FailNextPostBufferToPathCall() { countdown_to_postbuffer_fail_ = 1; }
  void FailNthPostBufferToPathCall(int n) { countdown_to_postbuffer_fail_ = n; }

  void SetKeystoreKey(const std::string& key);

  void FailNonPeriodicGetUpdates() { fail_non_periodic_get_updates_ = true; }

  
  bool client_stuck() const { return client_stuck_; }

  
  void SetGUClientCommand(sync_pb::ClientCommand* command);
  void SetCommitClientCommand(sync_pb::ClientCommand* command);

  void SetTransientErrorId(syncable::Id);

  const std::vector<syncable::Id>& committed_ids() const {
    return committed_ids_;
  }
  const std::vector<sync_pb::CommitMessage*>& commit_messages() const {
    return commit_messages_.get();
  }
  const std::vector<sync_pb::CommitResponse*>& commit_responses() const {
    return commit_responses_.get();
  }
  
  const sync_pb::CommitMessage& last_sent_commit() const;

  
  const sync_pb::CommitResponse& last_commit_response() const;

  
  const sync_pb::ClientToServerMessage& last_request() const;

  
  const std::vector<sync_pb::ClientToServerMessage>& requests() const;

  void set_conflict_all_commits(bool value) {
    conflict_all_commits_ = value;
  }
  void set_next_new_id(int value) {
    next_new_id_ = value;
  }
  void set_conflict_n_commits(int value) {
    conflict_n_commits_ = value;
  }

  void set_use_legacy_bookmarks_protocol(bool value) {
    use_legacy_bookmarks_protocol_ = value;
  }

  void set_store_birthday(std::string new_birthday) {
    
    
    base::AutoLock lock(store_birthday_lock_);
    store_birthday_ = new_birthday;
  }

  
  
  
  
  int GetAndClearNumGetUpdatesRequests() {
    int result = num_get_updates_requests_;
    num_get_updates_requests_ = 0;
    return result;
  }

  
  
  void ExpectGetUpdatesRequestTypes(ModelTypeSet expected_filter) {
    expected_filter_ = expected_filter;
  }

  void SetServerReachable();

  void SetServerNotReachable();

  
  
  
  
  void UpdateConnectionStatus();

  void SetServerStatus(HttpResponse::ServerConnectionCode server_status);

  
  const std::string store_birthday() {
    base::AutoLock lock(store_birthday_lock_);
    return store_birthday_;
  }

  
  void ClearUpdatesQueue() {
    update_queue_.clear();
  }

  
  sync_pb::SyncEntity* GetMutableLastUpdate();

 private:
  sync_pb::SyncEntity* AddUpdateFull(syncable::Id id, syncable::Id parentid,
                                     std::string name, int64 version,
                                     int64 sync_ts,
                                     bool is_dir);
  sync_pb::SyncEntity* AddUpdateFull(std::string id,
                                     std::string parentid, std::string name,
                                     int64 version, int64 sync_ts,
                                     bool is_dir);
  sync_pb::SyncEntity* AddUpdateMeta(std::string id, std::string parentid,
                                    std::string name, int64 version,
                                    int64 sync_ts);

  
  void ProcessGetUpdates(sync_pb::ClientToServerMessage* csm,
                         sync_pb::ClientToServerResponse* response);
  void ProcessCommit(sync_pb::ClientToServerMessage* csm,
                     sync_pb::ClientToServerResponse* response_buffer);
  void ProcessClearData(sync_pb::ClientToServerMessage* csm,
                        sync_pb::ClientToServerResponse* response);
  void AddDefaultBookmarkData(sync_pb::SyncEntity* entity, bool is_folder);

  
  bool ShouldConflictThisCommit();

  
  
  bool ShouldTransientErrorThisId(syncable::Id id);

  
  
  
  int64 GeneratePositionInParent() {
    return next_position_in_parent_--;
  }

  
  
  sync_pb::GetUpdatesResponse* GetUpdateResponse();
  void ApplyToken();

  
  
  
  bool IsModelTypePresentInSpecifics(
      const google::protobuf::RepeatedPtrField<
          sync_pb::DataTypeProgressMarker>& filter,
      ModelType value);

  sync_pb::DataTypeProgressMarker const* GetProgressMarkerForType(
      const google::protobuf::RepeatedPtrField<
          sync_pb::DataTypeProgressMarker>& filter,
      ModelType value);

  
  bool server_reachable_;

  
  std::vector<syncable::Id> committed_ids_;

  
  std::vector<syncable::Id> transient_error_ids_;

  
  bool conflict_all_commits_;
  int conflict_n_commits_;

  
  ScopedVector<sync_pb::CommitMessage> commit_messages_;
  ScopedVector<sync_pb::CommitResponse> commit_responses_;

  
  int next_new_id_;

  
  std::string store_birthday_;
  base::Lock store_birthday_lock_;
  bool store_birthday_sent_;
  bool client_stuck_;
  std::string commit_time_rename_prepended_string_;

  
  
  int countdown_to_postbuffer_fail_;

  
  
  
  syncable::Directory* directory_;

  
  std::list<sync_pb::GetUpdatesResponse> update_queue_;
  base::Closure mid_commit_callback_;
  MidCommitObserver* mid_commit_observer_;

  
  std::string keystore_key_;

  
  sync_pb::AuthenticateResponse auth_response_;
  
  std::string valid_auth_token_;

  
  
  bool throttling_;

  
  
  
  bool fail_with_auth_invalid_;

  base::Lock response_code_override_lock_;

  
  bool fail_non_periodic_get_updates_;

  scoped_ptr<sync_pb::ClientCommand> gu_client_command_;
  scoped_ptr<sync_pb::ClientCommand> commit_client_command_;

  
  int64 next_position_in_parent_;

  
  
  
  bool use_legacy_bookmarks_protocol_;

  ModelTypeSet expected_filter_;

  int num_get_updates_requests_;

  std::string next_token_;

  std::vector<sync_pb::ClientToServerMessage> requests_;

  DISALLOW_COPY_AND_ASSIGN(MockConnectionManager);
};

}  

#endif  
