// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_API_SYNC_DATA_H_
#define SYNC_API_SYNC_DATA_H_

#include <iosfwd>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/util/immutable.h"

namespace sync_pb {
class EntitySpecifics;
class SyncEntity;
}  

namespace syncer {

class SYNC_EXPORT SyncData {
 public:
  
  SyncData();
   ~SyncData();

   

  
  
  
  
  
  
  
  
  // overwritten if the datatype is encrypted.
  static SyncData CreateLocalDelete(
      const std::string& sync_tag,
      ModelType datatype);
  static SyncData CreateLocalData(
      const std::string& sync_tag,
      const std::string& non_unique_title,
      const sync_pb::EntitySpecifics& specifics);

  
  static SyncData CreateRemoteData(
      int64 id,
      const sync_pb::EntitySpecifics& specifics,
      const base::Time& last_modified_time);

  
  
  bool IsValid() const;

  
  
  ModelType GetDataType() const;

  
  const sync_pb::EntitySpecifics& GetSpecifics() const;

  
  
  const std::string& GetTag() const;

  
  
  const std::string& GetTitle() const;

  
  
  
  const base::Time& GetRemoteModifiedTime() const;

  
  int64 GetRemoteId() const;

  
  bool IsLocal() const;

  std::string ToString() const;

  

 private:
  
  
  struct ImmutableSyncEntityTraits {
    typedef sync_pb::SyncEntity* Wrapper;

    static void InitializeWrapper(Wrapper* wrapper);

    static void DestroyWrapper(Wrapper* wrapper);

    static const sync_pb::SyncEntity& Unwrap(const Wrapper& wrapper);

    static sync_pb::SyncEntity* UnwrapMutable(Wrapper* wrapper);

    static void Swap(sync_pb::SyncEntity* t1, sync_pb::SyncEntity* t2);
  };

  typedef Immutable<sync_pb::SyncEntity, ImmutableSyncEntityTraits>
      ImmutableSyncEntity;

  
  SyncData(int64 id,
           sync_pb::SyncEntity* entity,
           const base::Time& remote_modification_time);

  
  bool is_valid_;

  
  int64 id_;

  
  
  base::Time remote_modification_time_;

  
  ImmutableSyncEntity immutable_entity_;
};

void PrintTo(const SyncData& sync_data, std::ostream* os);

typedef std::vector<SyncData> SyncDataList;

}  

#endif  
