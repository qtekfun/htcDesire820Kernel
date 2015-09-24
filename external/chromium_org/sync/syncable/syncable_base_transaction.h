// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_SYNCABLE_BASE_TRANSACTION_H_
#define SYNC_SYNCABLE_SYNCABLE_BASE_TRANSACTION_H_

#include "base/location.h"
#include "sync/base/sync_export.h"
#include "sync/syncable/syncable_id.h"

namespace syncer {
namespace syncable {

class Directory;

enum WriterTag {
  INVALID,
  SYNCER,
  AUTHWATCHER,
  UNITTEST,
  VACUUM_AFTER_SAVE,
  HANDLE_SAVE_FAILURE,
  PURGE_ENTRIES,
  SYNCAPI,
};

std::string WriterTagToString(WriterTag writer_tag);

class SYNC_EXPORT BaseTransaction {
 public:
  static Id root_id();

  Directory* directory() const;

  virtual ~BaseTransaction();

  
  
  
  
  
  void OnUnrecoverableError(const tracked_objects::Location& location,
                            const std::string& message);

  bool unrecoverable_error_set() const;

 protected:
  BaseTransaction(const tracked_objects::Location& from_here,
                  const char* name,
                  WriterTag writer,
                  Directory* directory);

  void Lock();
  void Unlock();

  
  
  
  
  void HandleUnrecoverableErrorIfSet();

  const tracked_objects::Location from_here_;
  const char* const name_;
  WriterTag writer_;
  Directory* const directory_;

  
  bool unrecoverable_error_set_;
  tracked_objects::Location unrecoverable_error_location_;
  std::string unrecoverable_error_msg_;

 private:
  friend class Entry;
  DISALLOW_COPY_AND_ASSIGN(BaseTransaction);
};

}  
}  

#endif  
