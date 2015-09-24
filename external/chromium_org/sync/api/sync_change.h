// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_API_SYNC_CHANGE_H_
#define SYNC_API_SYNC_CHANGE_H_

#include <iosfwd>
#include <string>
#include <vector>

#include "base/location.h"
#include "sync/api/sync_data.h"
#include "sync/base/sync_export.h"

namespace syncer {

class SYNC_EXPORT SyncChange {
 public:
  enum SyncChangeType {
    ACTION_INVALID,
    ACTION_ADD,
    ACTION_UPDATE,
    ACTION_DELETE,
  };

  
  SyncChange();
  
  SyncChange(
      const tracked_objects::Location& from_here,
      SyncChangeType change_type,
      const SyncData& sync_data);
  ~SyncChange();

  

  
  
  
  
  
  
  bool IsValid() const;

  
  SyncChangeType change_type() const;
  SyncData sync_data() const;
  tracked_objects::Location location() const;

  
  static std::string ChangeTypeToString(SyncChangeType change_type);

  
  
  std::string ToString() const;

 private:
  tracked_objects::Location location_;

  SyncChangeType change_type_;

  
  
  SyncData sync_data_;
};

SYNC_EXPORT void PrintTo(const SyncChange& sync_change, std::ostream* os);

}  

#endif  
