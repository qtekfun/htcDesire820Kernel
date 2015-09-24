// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_BASE_NODE_H_
#define SYNC_INTERNAL_API_PUBLIC_BASE_NODE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/protocol/sync.pb.h"
#include "url/gurl.h"

namespace base {
class DictionaryValue;
}

namespace sync_pb {
class AppSpecifics;
class AutofillSpecifics;
class AutofillProfileSpecifics;
class BookmarkSpecifics;
class EntitySpecifics;
class ExtensionSpecifics;
class SessionSpecifics;
class NigoriSpecifics;
class PreferenceSpecifics;
class PasswordSpecificsData;
class ThemeSpecifics;
class TypedUrlSpecifics;
}

namespace syncer {

class BaseTransaction;

namespace syncable {
class BaseTransaction;
class Entry;
}

static const int64 kInvalidId = 0;

class SYNC_EXPORT BaseNode {
 public:
  
  enum InitByLookupResult {
    INIT_OK,
    
    INIT_FAILED_ENTRY_NOT_GOOD,
    
    INIT_FAILED_ENTRY_IS_DEL,
    
    INIT_FAILED_DECRYPT_IF_NECESSARY,
    
    
    INIT_FAILED_PRECONDITION,
  };

  
  
  
  virtual InitByLookupResult InitByIdLookup(int64 id) = 0;

  
  
  
  virtual InitByLookupResult InitByClientTagLookup(
      ModelType model_type,
      const std::string& tag) = 0;

  
  
  
  
  virtual int64 GetId() const;

  
  base::Time GetModificationTime() const;

  
  
  
  int64 GetParentId() const;

  
  
  bool GetIsFolder() const;

  
  
  
  std::string GetTitle() const;

  
  
  ModelType GetModelType() const;

  
  
  const sync_pb::BookmarkSpecifics& GetBookmarkSpecifics() const;

  
  
  const sync_pb::AppSpecifics& GetAppSpecifics() const;

  
  
  const sync_pb::AutofillSpecifics& GetAutofillSpecifics() const;

  virtual const sync_pb::AutofillProfileSpecifics&
      GetAutofillProfileSpecifics() const;

  
  
  const sync_pb::NigoriSpecifics& GetNigoriSpecifics() const;

  
  
  const sync_pb::PasswordSpecificsData& GetPasswordSpecifics() const;

  
  
  const sync_pb::PreferenceSpecifics& GetPreferenceSpecifics() const;

  
  
  const sync_pb::ThemeSpecifics& GetThemeSpecifics() const;

  
  
  const sync_pb::TypedUrlSpecifics& GetTypedUrlSpecifics() const;

  
  
  const sync_pb::ExtensionSpecifics& GetExtensionSpecifics() const;

  
  
  const sync_pb::SessionSpecifics& GetSessionSpecifics() const;

  
  
  const sync_pb::ManagedUserSettingSpecifics&
      GetManagedUserSettingSpecifics() const;

  
  
  const sync_pb::ManagedUserSpecifics& GetManagedUserSpecifics() const;

  
  
  const sync_pb::DeviceInfoSpecifics& GetDeviceInfoSpecifics() const;

  
  
  const sync_pb::ExperimentsSpecifics& GetExperimentsSpecifics() const;

  
  
  const sync_pb::PriorityPreferenceSpecifics&
      GetPriorityPreferenceSpecifics() const;

  const sync_pb::EntitySpecifics& GetEntitySpecifics() const;

  
  int64 GetExternalId() const;

  
  bool HasChildren() const;

  
  
  int64 GetPredecessorId() const;

  
  
  int64 GetSuccessorId() const;

  
  
  int64 GetFirstChildId() const;

  
  
  
  void GetChildIds(std::vector<int64>* result) const;

  
  
  int GetTotalNodeCount() const;

  
  
  
  int GetPositionIndex() const;

  
  virtual const syncable::Entry* GetEntry() const = 0;
  virtual const BaseTransaction* GetTransaction() const = 0;

  
  
  base::DictionaryValue* GetSummaryAsValue() const;

  
  
  base::DictionaryValue* GetDetailsAsValue() const;

 protected:
  BaseNode();
  virtual ~BaseNode();

  
  
  
  
  
  bool DecryptIfNecessary();

  
  
  
  const sync_pb::EntitySpecifics& GetUnencryptedSpecifics(
      const syncable::Entry* entry) const;

  
  void SetUnencryptedSpecifics(const sync_pb::EntitySpecifics& specifics);

 private:
  
  
  friend class SyncManagerTest;
  FRIEND_TEST_ALL_PREFIXES(SyncApiTest, GenerateSyncableHash);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, UpdateEntryWithEncryption);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest,
                           UpdatePasswordSetEntitySpecificsNoChange);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, UpdatePasswordSetPasswordSpecifics);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, UpdatePasswordNewPassphrase);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, UpdatePasswordReencryptEverything);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, SetBookmarkTitle);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, SetBookmarkTitleWithEncryption);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, SetNonBookmarkTitle);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, SetNonBookmarkTitleWithEncryption);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, SetPreviouslyEncryptedSpecifics);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, IncrementTransactionVersion);

  void* operator new(size_t size);  

  
  sync_pb::EntitySpecifics unencrypted_data_;

  
  scoped_ptr<sync_pb::PasswordSpecificsData> password_data_;

  DISALLOW_COPY_AND_ASSIGN(BaseNode);
};

}  

#endif  
