// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_DATA_TYPE_DEBUG_INFO_LISTENER_H_
#define SYNC_INTERNAL_API_PUBLIC_DATA_TYPE_DEBUG_INFO_LISTENER_H_

#include <vector>

#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/data_type_association_stats.h"

namespace syncer {

struct SYNC_EXPORT DataTypeConfigurationStats {
  DataTypeConfigurationStats();
  ~DataTypeConfigurationStats();

  
  ModelType model_type;

  
  base::TimeDelta download_wait_time;

  
  base::TimeDelta download_time;

  
  
  base::TimeDelta association_wait_time_for_high_priority;

  
  ModelTypeSet high_priority_types_configured_before;
  ModelTypeSet same_priority_types_configured_before;

  
  DataTypeAssociationStats association_stats;
};

class DataTypeDebugInfoListener {
 public:
  
  virtual void OnDataTypeConfigureComplete(
      const std::vector<DataTypeConfigurationStats>& configuration_stats) = 0;
};

}  

#endif  
