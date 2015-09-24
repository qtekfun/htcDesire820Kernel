// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_WRITE_NODE_H_
#define SYNC_INTERNAL_API_PUBLIC_WRITE_NODE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/base_node.h"

namespace sync_pb {
class AppSpecifics;
class AutofillSpecifics;
class AutofillProfileSpecifics;
class BookmarkSpecifics;
class EntitySpecifics;
class ExtensionSpecifics;
class SessionSpecifics;
class NigoriSpecifics;
class PasswordSpecificsData;
class ThemeSpecifics;
class TypedUrlSpecifics;
}

namespace syncer {

class Cryptographer;
class WriteTransaction;

namespace syncable {
class Entry;
class MutableEntry;
}

class SYNC_EXPORT WriteNode : public BaseNode {
 public:
  enum InitUniqueByCreationResult {
    INIT_SUCCESS,
    
    INIT_FAILED_EMPTY_TAG,
    
    INIT_FAILED_ENTRY_ALREADY_EXISTS,
    
    INIT_FAILED_COULD_NOT_CREATE_ENTRY,
    
    INIT_FAILED_SET_PREDECESSOR,
  };

  
  explicit WriteNode(WriteTransaction* transaction);
  virtual ~WriteNode();

  
  

  
  virtual InitByLookupResult InitByIdLookup(int64 id) OVERRIDE;
  virtual InitByLookupResult InitByClientTagLookup(
      ModelType model_type,
      const std::string& tag) OVERRIDE;

  
  
  
  
  bool InitBookmarkByCreation(const BaseNode& parent,
                              const BaseNode* predecessor);

  
  
  
  
  
  
  InitUniqueByCreationResult InitUniqueByCreation(
      ModelType model_type,
      const BaseNode& parent,
      const std::string& client_tag);

  
  
  
  InitByLookupResult InitByTagLookup(const std::string& tag);

  
  void SetIsFolder(bool folder);
  void SetTitle(const std::wstring& title);

  
  
  void SetExternalId(int64 external_id);

  
  void Tombstone();

  
  
  
  void Drop();

  
  
  
  bool SetPosition(const BaseNode& new_parent, const BaseNode* predecessor);

  
  
  void SetBookmarkSpecifics(const sync_pb::BookmarkSpecifics& specifics);

  
  void SetEntitySpecifics(const sync_pb::EntitySpecifics& specifics);

  
  
  void ResetFromSpecifics();

  
  

  
  
  void SetAppSpecifics(const sync_pb::AppSpecifics& specifics);

  
  
  void SetAutofillSpecifics(const sync_pb::AutofillSpecifics& specifics);

  void SetAutofillProfileSpecifics(
      const sync_pb::AutofillProfileSpecifics& specifics);

  
  
  void SetNigoriSpecifics(const sync_pb::NigoriSpecifics& specifics);

  
  
  void SetPasswordSpecifics(const sync_pb::PasswordSpecificsData& specifics);

  
  
  void SetThemeSpecifics(const sync_pb::ThemeSpecifics& specifics);

  
  
  void SetTypedUrlSpecifics(const sync_pb::TypedUrlSpecifics& specifics);

  
  
  void SetExtensionSpecifics(const sync_pb::ExtensionSpecifics& specifics);

  
  
  void SetSessionSpecifics(const sync_pb::SessionSpecifics& specifics);

  
  
  void SetManagedUserSettingSpecifics(
      const sync_pb::ManagedUserSettingSpecifics& specifics);

  
  
  void SetManagedUserSpecifics(const sync_pb::ManagedUserSpecifics& specifics);

  
  
  void SetDeviceInfoSpecifics(const sync_pb::DeviceInfoSpecifics& specifics);

  
  
  void SetExperimentsSpecifics(const sync_pb::ExperimentsSpecifics& specifics);

  
  
  void SetPriorityPreferenceSpecifics(
      const sync_pb::PriorityPreferenceSpecifics& specifics);

  
  virtual const syncable::Entry* GetEntry() const OVERRIDE;

  virtual const BaseTransaction* GetTransaction() const OVERRIDE;

  syncable::MutableEntry* GetMutableEntryForTest();

 private:
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, EncryptBookmarksWithLegacyData);

  void* operator new(size_t size);  

  
  bool PutPredecessor(const BaseNode* predecessor) WARN_UNUSED_RESULT;

  
  
  void MarkForSyncing();

  
  syncable::MutableEntry* entry_;

  
  WriteTransaction* transaction_;

  DISALLOW_COPY_AND_ASSIGN(WriteNode);
};

}  

#endif  
