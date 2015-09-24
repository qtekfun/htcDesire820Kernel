// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_NIGORI_HANDLER_H_
#define SYNC_SYNCABLE_NIGORI_HANDLER_H_

#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"

namespace google{
namespace protobuf{
template <typename T>
class RepeatedPtrField;
}
}

namespace sync_pb {
class NigoriSpecifics;
}

namespace syncer {
namespace syncable {

class BaseTransaction;

class SYNC_EXPORT_PRIVATE NigoriHandler {
 public:
  NigoriHandler();
  virtual ~NigoriHandler();

  
  
  virtual void ApplyNigoriUpdate(
      const sync_pb::NigoriSpecifics& nigori,
      syncable::BaseTransaction* const trans) = 0;

  
  virtual void UpdateNigoriFromEncryptedTypes(
      sync_pb::NigoriSpecifics* nigori,
      syncable::BaseTransaction* const trans) const = 0;

  
  virtual bool NeedKeystoreKey(
      syncable::BaseTransaction* const trans) const = 0;

  
  
  virtual bool SetKeystoreKeys(
      const google::protobuf::RepeatedPtrField<std::string>& keys,
      syncable::BaseTransaction* const trans) = 0;

  
  virtual ModelTypeSet GetEncryptedTypes(
      syncable::BaseTransaction* const trans) const = 0;
};

}  
}  

#endif  
