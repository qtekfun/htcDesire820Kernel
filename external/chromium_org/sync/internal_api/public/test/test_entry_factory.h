// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_TEST_TEST_ENTRY_FACTORY_H_
#define SYNC_TEST_TEST_ENTRY_FACTORY_H_

#include <string>

#include "base/basictypes.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/protocol/sync.pb.h"

namespace syncer {

namespace syncable {
class Directory;
class Id;
}

class TestEntryFactory {
 public:
  explicit TestEntryFactory(syncable::Directory* dir);
  ~TestEntryFactory();

  
  int64 CreateUnappliedNewItemWithParent(
      const std::string& item_id,
      const sync_pb::EntitySpecifics& specifics,
      const std::string& parent_id);

  int64 CreateUnappliedNewBookmarkItemWithParent(
      const std::string& item_id,
      const sync_pb::EntitySpecifics& specifics,
      const std::string& parent_id);

  
  int64 CreateUnappliedNewItem(const std::string& item_id,
                               const sync_pb::EntitySpecifics& specifics,
                               bool is_unique);

  
  
  
  
  
  void CreateUnsyncedItem(const syncable::Id& item_id,
                          const syncable::Id& parent_id,
                          const std::string& name,
                          bool is_folder,
                          ModelType model_type,
                          int64* metahandle_out);

  
  
  int64 CreateUnappliedAndUnsyncedBookmarkItem(const std::string& name);

  
  
  
  int64 CreateSyncedItem(const std::string& name,
                         ModelType model_type, bool is_folder);

  
  
  
  int64 CreateUnappliedRootNode(ModelType model_type);

  
  
  
  
  bool SetServerSpecificsForItem(int64 meta_handle,
                                 const sync_pb::EntitySpecifics specifics);

  
  
  
  
  bool SetLocalSpecificsForItem(int64 meta_handle,
                                const sync_pb::EntitySpecifics specifics);

  
  
  const sync_pb::EntitySpecifics& GetServerSpecificsForItem(
      int64 meta_handle) const;

  
  
  const sync_pb::EntitySpecifics& GetLocalSpecificsForItem(
      int64 meta_handle) const;

  
  bool GetIsUnsyncedForItem(int64 meta_handle) const;
  bool GetIsUnappliedForItem(int64 meta_handle) const;

  int64 GetNextRevision();

 private:
  syncable::Directory* directory_;
  int64 next_revision_;

  DISALLOW_COPY_AND_ASSIGN(TestEntryFactory);
};

}  

#endif  
