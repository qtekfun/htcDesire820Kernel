// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_NOTIFIER_UNACKED_INVALIDATION_SET_H_
#define SYNC_NOTIFIER_UNACKED_INVALIDATION_SET_H_

#include <vector>

#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/invalidation.h"
#include "sync/internal_api/public/util/weak_handle.h"
#include "sync/notifier/invalidation_util.h"

namespace base {
class DictionaryValue;
}  

namespace syncer {

namespace test_util {
class UnackedInvalidationSetEqMatcher;
}  

class SingleObjectInvalidationSet;
class ObjectIdInvalidationMap;
class AckHandle;

class SYNC_EXPORT UnackedInvalidationSet {
 public:
  static const size_t kMaxBufferedInvalidations;

  UnackedInvalidationSet(invalidation::ObjectId id);
  ~UnackedInvalidationSet();

  
  const invalidation::ObjectId& object_id() const;

  
  void Add(const Invalidation& invalidation);

  
  void AddSet(const SingleObjectInvalidationSet& invalidations);

  
  
  
  
  
  
  
  void ExportInvalidations(WeakHandle<AckHandler> ack_handler,
                           ObjectIdInvalidationMap* out) const;

  
  void Clear();

  
  
  
  
  
  
  
  
  
  
  
  void SetHandlerIsRegistered();

  
  
  
  
  void SetHandlerIsUnregistered();

  
  
  
  void Acknowledge(const AckHandle& handle);

  
  
  
  void Drop(const AckHandle& handle);

  scoped_ptr<base::DictionaryValue> ToValue() const;
  bool ResetFromValue(const base::DictionaryValue& value);

 private:
  
  friend class test_util::UnackedInvalidationSetEqMatcher;

  typedef std::set<Invalidation, InvalidationVersionLessThan> InvalidationsSet;

  bool ResetListFromValue(const base::ListValue& value);

  
  
  
  void Truncate(size_t max_size);

  bool registered_;
  invalidation::ObjectId object_id_;
  InvalidationsSet invalidations_;
};

typedef std::map<invalidation::ObjectId,
                 UnackedInvalidationSet,
                 ObjectIdLessThan> UnackedInvalidationsMap;

}  

#endif  
