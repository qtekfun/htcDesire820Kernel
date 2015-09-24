// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SESSIONS_DEBUG_INFO_GETTER_H_
#define SYNC_SESSIONS_DEBUG_INFO_GETTER_H_

#include "sync/base/sync_export.h"
#include "sync/protocol/sync.pb.h"

namespace syncer {
namespace sessions {

class SYNC_EXPORT_PRIVATE DebugInfoGetter {
 public:
  
  
  virtual void GetDebugInfo(sync_pb::DebugInfo* debug_info) = 0;

  
  virtual void ClearDebugInfo() = 0;

  virtual ~DebugInfoGetter() {}
};

}  
}  

#endif  
