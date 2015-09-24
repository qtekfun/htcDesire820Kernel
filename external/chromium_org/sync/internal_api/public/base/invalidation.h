// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_BASE_INVALIDATION_H_
#define SYNC_INTERNAL_API_PUBLIC_BASE_INVALIDATION_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "google/cacheinvalidation/include/types.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/ack_handle.h"
#include "sync/internal_api/public/util/weak_handle.h"

namespace syncer {

class DroppedInvalidationTracker;
class AckHandler;

class SYNC_EXPORT Invalidation {
 public:
  
  static Invalidation Init(
      const invalidation::ObjectId& id,
      int64 version,
      const std::string& payload);
  static Invalidation InitUnknownVersion(const invalidation::ObjectId& id);
  static Invalidation InitFromDroppedInvalidation(const Invalidation& dropped);
  static scoped_ptr<Invalidation> InitFromValue(
      const base::DictionaryValue& value);

  ~Invalidation();

  
  bool Equals(const Invalidation& other) const;

  invalidation::ObjectId object_id() const;
  bool is_unknown_version() const;

  
  int64 version() const;

  
  const std::string& payload() const;

  const AckHandle& ack_handle() const;

  
  
  
  
  
  
  
  void set_ack_handler(syncer::WeakHandle<AckHandler> ack_handler);

  
  
  
  
  
  
  
  bool SupportsAcknowledgement() const;

  
  
  
  
  
  
  void Acknowledge() const;

  
  
  
  
  
  
  
  
  
  
  
  
  void Drop(DroppedInvalidationTracker* tracker) const;

  scoped_ptr<base::DictionaryValue> ToValue() const;
  std::string ToString() const;

 private:
  Invalidation(const invalidation::ObjectId& id,
               bool is_unknown_version,
               int64 version,
               const std::string& payload,
               AckHandle ack_handle);

  
  invalidation::ObjectId id_;

  
  bool is_unknown_version_;

  
  
  int64 version_;

  
  
  std::string payload_;

  
  AckHandle ack_handle_;
  syncer::WeakHandle<AckHandler> ack_handler_;
};

}  

#endif  
