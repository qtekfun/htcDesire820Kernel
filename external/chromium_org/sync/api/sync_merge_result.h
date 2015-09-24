// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_API_SYNC_MERGE_RESULT_H_
#define SYNC_API_SYNC_MERGE_RESULT_H_

#include "sync/api/sync_error.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"

namespace syncer {

class SYNC_EXPORT SyncMergeResult {
 public:
  
  explicit SyncMergeResult(ModelType type);
  ~SyncMergeResult();

  

  
  
  void set_error(SyncError error);
  void set_num_items_before_association(int num_items_before_association);
  void set_num_items_after_association(int num_items_after_association);
  void set_num_items_added(int num_items_added);
  void set_num_items_deleted(int num_items_deleted);
  void set_num_items_modified(int num_items_modified);
  void set_pre_association_version(int64 version);

  
  ModelType model_type() const;
  SyncError error() const;
  int num_items_before_association() const;
  int num_items_after_association() const;
  int num_items_added() const;
  int num_items_deleted() const;
  int num_items_modified() const;
  int64 pre_association_version() const;

 private:
  
  void CopyFrom(const SyncMergeResult& other);

  
  ModelType model_type_;

  
  
  SyncError error_;

  
  int num_items_before_association_;

  
  int num_items_after_association_;

  
  
  int num_items_added_;
  int num_items_deleted_;
  int num_items_modified_;

  
  int64 pre_association_version_;
};

}  

#endif  
