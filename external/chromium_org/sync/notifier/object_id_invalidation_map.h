// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_NOTIFIER_OBJECT_ID_INVALIDATION_MAP_H_
#define SYNC_NOTIFIER_OBJECT_ID_INVALIDATION_MAP_H_

#include <map>
#include <vector>

#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/invalidation.h"
#include "sync/notifier/invalidation_util.h"
#include "sync/notifier/single_object_invalidation_set.h"

namespace syncer {

class SYNC_EXPORT ObjectIdInvalidationMap {
  public:
   
   
   static ObjectIdInvalidationMap InvalidateAll(const ObjectIdSet& ids);

   ObjectIdInvalidationMap();
   ~ObjectIdInvalidationMap();

   
   ObjectIdSet GetObjectIds() const;

   
   bool Empty() const;

   
   bool operator==(const ObjectIdInvalidationMap& other) const;

   
   void Insert(const Invalidation& invalidation);

   
   
   ObjectIdInvalidationMap GetSubsetWithObjectIds(const ObjectIdSet& ids) const;

   
   const SingleObjectInvalidationSet& ForObject(
       invalidation::ObjectId id) const;

   
   void GetAllInvalidations(std::vector<syncer::Invalidation>* out) const;

   
   void AcknowledgeAll() const;

   
   scoped_ptr<base::ListValue> ToValue() const;

   
   bool ResetFromValue(const base::ListValue& value);

   
   std::string ToString() const;

  private:
   typedef std::map<invalidation::ObjectId,
                    SingleObjectInvalidationSet,
                    ObjectIdLessThan> IdToListMap;

   ObjectIdInvalidationMap(const IdToListMap& map);

   IdToListMap map_;
};

}  

#endif  
