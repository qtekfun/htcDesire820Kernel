// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_DATA_TYPE_ASSOCIATION_STATS_H_
#define SYNC_INTERNAL_API_PUBLIC_DATA_TYPE_ASSOCIATION_STATS_H_

#include "base/time/time.h"
#include "sync/base/sync_export.h"

namespace syncer {

struct SYNC_EXPORT DataTypeAssociationStats {
  DataTypeAssociationStats();
  ~DataTypeAssociationStats();

  
  int num_local_items_before_association;
  int num_sync_items_before_association;

  
  int num_local_items_after_association;
  int num_sync_items_after_association;

  
  
  int num_local_items_added;
  int num_local_items_deleted;
  int num_local_items_modified;
  int num_sync_items_added;
  int num_sync_items_deleted;
  int num_sync_items_modified;

  
  int64 local_version_pre_association;
  int64 sync_version_pre_association;

  
  bool had_error;

  
  
  base::TimeDelta association_wait_time;

  
  base::TimeDelta association_time;
};

}  

#endif  
