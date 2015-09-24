// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_MODEL_NEUTRAL_MUTABLE_ENTRY_H_
#define SYNC_SYNCABLE_MODEL_NEUTRAL_MUTABLE_ENTRY_H_

#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/syncable/entry.h"

namespace syncer {
class WriteNode;

namespace syncable {

class BaseWriteTransaction;

enum CreateNewUpdateItem {
  CREATE_NEW_UPDATE_ITEM
};

class SYNC_EXPORT_PRIVATE ModelNeutralMutableEntry : public Entry {
 public:
  ModelNeutralMutableEntry(BaseWriteTransaction* trans,
                           CreateNewUpdateItem,
                           const Id& id);
  ModelNeutralMutableEntry(BaseWriteTransaction* trans, GetByHandle, int64);
  ModelNeutralMutableEntry(BaseWriteTransaction* trans, GetById, const Id&);
  ModelNeutralMutableEntry(
      BaseWriteTransaction* trans,
      GetByClientTag,
      const std::string& tag);
  ModelNeutralMutableEntry(
      BaseWriteTransaction* trans,
      GetByServerTag,
      const std::string& tag);

  inline BaseWriteTransaction* base_write_transaction() const {
    return base_write_transaction_;
  }

  
  
  
  
  
  
  
  
  
  void PutBaseVersion(int64 value);
  void PutServerVersion(int64 value);
  void PutServerMtime(base::Time value);
  void PutServerCtime(base::Time value);
  bool PutId(const Id& value);
  void PutServerParentId(const Id& value);
  bool PutIsUnsynced(bool value);
  bool PutIsUnappliedUpdate(bool value);
  void PutServerIsDir(bool value);
  void PutServerIsDel(bool value);
  void PutServerNonUniqueName(const std::string& value);
  bool PutUniqueServerTag(const std::string& value);
  bool PutUniqueClientTag(const std::string& value);
  void PutUniqueBookmarkTag(const std::string& tag);
  void PutServerSpecifics(const sync_pb::EntitySpecifics& value);
  void PutBaseServerSpecifics(const sync_pb::EntitySpecifics& value);
  void PutServerUniquePosition(const UniquePosition& value);
  void PutSyncing(bool value);

  
  
  
  
  
  
  
  
  
  void PutParentIdPropertyOnly(const Id& parent_id);

  
  
  
  
  void UpdateTransactionVersion(int64 version);

 protected:
  explicit ModelNeutralMutableEntry(BaseWriteTransaction* trans);

  syncable::MetahandleSet* GetDirtyIndexHelper();

 private:
  friend class syncer::WriteNode;
  friend class Directory;

  
  void* operator new(size_t size) { return (::operator new)(size); }

  
  
  
  BaseWriteTransaction* const base_write_transaction_;

  DISALLOW_COPY_AND_ASSIGN(ModelNeutralMutableEntry);
};

}  
}  

#endif  
