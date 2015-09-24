// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_UTIL_TIME_H_
#define SYNC_UTIL_TIME_H_

#include <string>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "sync/base/sync_export.h"

namespace syncer {

SYNC_EXPORT int64 TimeToProtoTime(const base::Time& t);

SYNC_EXPORT_PRIVATE base::Time ProtoTimeToTime(int64 proto_t);

SYNC_EXPORT std::string GetTimeDebugString(const base::Time& t);

}  

#endif  
