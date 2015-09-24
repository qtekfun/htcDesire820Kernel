// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_MUTABLE_ENTRY_H_
#define SYNC_SYNCABLE_MUTABLE_ENTRY_H_

#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/syncable/entry.h"
#include "sync/syncable/metahandle_set.h"
#include "sync/syncable/model_neutral_mutable_entry.h"

namespace syncer {
class WriteNode;

namespace syncable {

enum Create {
  CREATE
};

class WriteTransaction;

class SYNC_EXPORT_PRIVATE MutableEntry : public ModelNeutralMutableEntry {
  void Init(WriteTransaction* trans, ModelType model_type,
            const Id& parent_id, const std::string& name);

 public:
  MutableEntry(WriteTransaction* trans, CreateNewUpdateItem, const Id& id);
  MutableEntry(WriteTransaction* trans, Create, ModelType model_type,
               const Id& parent_id, const std::string& name);
  MutableEntry(WriteTransaction* trans, GetByHandle, int64);
  MutableEntry(WriteTransaction* trans, GetById, const Id&);
  MutableEntry(WriteTransaction* trans, GetByClientTag, const std::string& tag);
  MutableEntry(WriteTransaction* trans, GetByServerTag, const std::string& tag);

  inline WriteTransaction* write_transaction() const {
    return write_transaction_;
  }

  
  
  void PutLocalExternalId(int64 value);
  void PutMtime(base::Time value);
  void PutCtime(base::Time value);
  void PutParentId(const Id& value);
  void PutIsDir(bool value);
  void PutIsDel(bool value);
  void PutNonUniqueName(const std::string& value);
  void PutSpecifics(const sync_pb::EntitySpecifics& value);
  void PutUniquePosition(const UniquePosition& value);

  
  
  
  
  bool PutPredecessor(const Id& predecessor_id);

 private:
  
  
  
  WriteTransaction* const write_transaction_;

  DISALLOW_COPY_AND_ASSIGN(MutableEntry);
};

bool MarkForSyncing(syncable::MutableEntry* e);

}  
}  

#endif  
