// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_SYNCER_PROTO_UTIL_H_
#define SYNC_ENGINE_SYNCER_PROTO_UTIL_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/time/time.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/util/syncer_error.h"
#include "sync/sessions/sync_session.h"
#include "sync/syncable/blob.h"

namespace sync_pb {
class ClientToServerMessage;
class ClientToServerResponse;
class ClientToServerResponse_Error;
class CommitResponse_EntryResponse;
class EntitySpecifics;
class SyncEntity;
}

namespace syncer {

class ServerConnectionManager;

namespace sessions {
class SyncProtocolError;
class SyncSessionContext;
}

namespace syncable {
class Directory;
class Entry;
}

SYNC_EXPORT_PRIVATE ModelTypeSet GetTypesToMigrate(
    const sync_pb::ClientToServerResponse& response);

SYNC_EXPORT_PRIVATE SyncProtocolError ConvertErrorPBToLocalType(
    const sync_pb::ClientToServerResponse_Error& error);

class SYNC_EXPORT_PRIVATE SyncerProtoUtil {
 public:
  
  
  
  
  
  static SyncerError PostClientToServerMessage(
      sync_pb::ClientToServerMessage* msg,
      sync_pb::ClientToServerResponse* response,
      sessions::SyncSession* session);

  static bool ShouldMaintainPosition(const sync_pb::SyncEntity& sync_entity);

  
  
  static void CopyProtoBytesIntoBlob(const std::string& proto_bytes,
                                     syncable::Blob* blob);
  static bool ProtoBytesEqualsBlob(const std::string& proto_bytes,
                                   const syncable::Blob& blob);
  static void CopyBlobIntoProtoBytes(const syncable::Blob& blob,
                                     std::string* proto_bytes);

  
  static const std::string& NameFromSyncEntity(
      const sync_pb::SyncEntity& entry);

  
  static const std::string& NameFromCommitEntryResponse(
      const sync_pb::CommitResponse_EntryResponse& entry);

  
  static void PersistBagOfChips(
      syncable::Directory* dir,
      const sync_pb::ClientToServerResponse& response);

  
  
  
  static void AddToEntitySpecificDatatypesFilter(ModelType datatype,
      sync_pb::EntitySpecifics* filter);

  
  static std::string ClientToServerResponseDebugString(
      const sync_pb::ClientToServerResponse& response);

  
  
  static std::string SyncEntityDebugString(const sync_pb::SyncEntity& entry);

  
  static void AddRequestBirthday(syncable::Directory* dir,
                                 sync_pb::ClientToServerMessage* msg);

  
  static void AddBagOfChips(syncable::Directory* dir,
                            sync_pb::ClientToServerMessage* msg);


  
  static void SetProtocolVersion(sync_pb::ClientToServerMessage* msg);

 private:
  SyncerProtoUtil() {}

  

  
  
  static bool VerifyResponseBirthday(
      const sync_pb::ClientToServerResponse& response,
      syncable::Directory* dir);

  
  static bool IsSyncDisabledByAdmin(
      const sync_pb::ClientToServerResponse& response);

  
  
  static bool PostAndProcessHeaders(ServerConnectionManager* scm,
                                    sessions::SyncSession* session,
                                    const sync_pb::ClientToServerMessage& msg,
                                    sync_pb::ClientToServerResponse* response);

  static base::TimeDelta GetThrottleDelay(
      const sync_pb::ClientToServerResponse& response);

  friend class SyncerProtoUtilTest;
  FRIEND_TEST_ALL_PREFIXES(SyncerProtoUtilTest, AddRequestBirthday);
  FRIEND_TEST_ALL_PREFIXES(SyncerProtoUtilTest, PostAndProcessHeaders);
  FRIEND_TEST_ALL_PREFIXES(SyncerProtoUtilTest, VerifyDisabledByAdmin);
  FRIEND_TEST_ALL_PREFIXES(SyncerProtoUtilTest, VerifyResponseBirthday);
  FRIEND_TEST_ALL_PREFIXES(SyncerProtoUtilTest, HandleThrottlingNoDatatypes);
  FRIEND_TEST_ALL_PREFIXES(SyncerProtoUtilTest, HandleThrottlingWithDatatypes);

  DISALLOW_COPY_AND_ASSIGN(SyncerProtoUtil);
};

}  

#endif  
