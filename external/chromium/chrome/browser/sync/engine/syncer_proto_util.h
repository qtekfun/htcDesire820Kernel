// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_SYNCER_PROTO_UTIL_H_
#define CHROME_BROWSER_SYNC_ENGINE_SYNCER_PROTO_UTIL_H_
#pragma once

#include <string>

#include "base/gtest_prod_util.h"
#include "chrome/browser/sync/syncable/blob.h"
#include "chrome/browser/sync/syncable/model_type.h"

namespace syncable {
class Directory;
class Entry;
class ScopedDirLookup;
class SyncName;
}  

namespace sync_pb {
class ClientToServerResponse;
class EntitySpecifics;
}  

namespace browser_sync {

namespace sessions {
class SyncSession;
}

class AuthWatcher;
class ClientToServerMessage;
class ServerConnectionManager;
class SyncEntity;
class CommitResponse_EntryResponse;

class SyncerProtoUtil {
 public:
  
  
  
  
  static bool PostClientToServerMessage(
      const ClientToServerMessage& msg,
      sync_pb::ClientToServerResponse* response,
      sessions::SyncSession* session);

  
  
  
  
  
  
  
  static bool Compare(const syncable::Entry& local_entry,
                      const SyncEntity& server_entry);

  
  
  static void CopyProtoBytesIntoBlob(const std::string& proto_bytes,
                                     syncable::Blob* blob);
  static bool ProtoBytesEqualsBlob(const std::string& proto_bytes,
                                   const syncable::Blob& blob);
  static void CopyBlobIntoProtoBytes(const syncable::Blob& blob,
                                     std::string* proto_bytes);

  
  static const std::string& NameFromSyncEntity(
      const sync_pb::SyncEntity& entry);

  
  static const std::string& NameFromCommitEntryResponse(
      const CommitResponse_EntryResponse& entry);

  
  
  
  static void AddToEntitySpecificDatatypesFilter(syncable::ModelType datatype,
      sync_pb::EntitySpecifics* filter);

  
  static std::string ClientToServerResponseDebugString(
      const sync_pb::ClientToServerResponse& response);

  
  
  static std::string SyncEntityDebugString(const sync_pb::SyncEntity& entry);

  
  static void AddRequestBirthday(syncable::Directory* dir,
                                 ClientToServerMessage* msg);

 private:
  SyncerProtoUtil() {}

  

  
  
  static bool VerifyResponseBirthday(syncable::Directory* dir,
      const sync_pb::ClientToServerResponse* response);

  
  
  static void HandleMigrationDoneResponse(
      const sync_pb::ClientToServerResponse* response,
      sessions::SyncSession* session);

  
  
  static bool PostAndProcessHeaders(browser_sync::ServerConnectionManager* scm,
                                    sessions::SyncSession* session,
                                    const ClientToServerMessage& msg,
                                    sync_pb::ClientToServerResponse* response);

  friend class SyncerProtoUtilTest;
  FRIEND_TEST_ALL_PREFIXES(SyncerProtoUtilTest, AddRequestBirthday);
  FRIEND_TEST_ALL_PREFIXES(SyncerProtoUtilTest, PostAndProcessHeaders);
  FRIEND_TEST_ALL_PREFIXES(SyncerProtoUtilTest, VerifyResponseBirthday);

  DISALLOW_COPY_AND_ASSIGN(SyncerProtoUtil);
};

}  

#endif  
