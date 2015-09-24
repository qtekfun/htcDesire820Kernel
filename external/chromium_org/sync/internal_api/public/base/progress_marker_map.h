// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_PUBLIC_API_BASE_PROGRESS_MARKER_MAP_H_
#define SYNC_INTERNAL_PUBLIC_API_BASE_PROGRESS_MARKER_MAP_H_

#include <map>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"


namespace base {
class DictionaryValue;
}

namespace syncer {

typedef std::map<ModelType, std::string> ProgressMarkerMap;

SYNC_EXPORT_PRIVATE scoped_ptr<base::DictionaryValue> ProgressMarkerMapToValue(
    const ProgressMarkerMap& marker_map);

}  

#endif  
