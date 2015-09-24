// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_PUBLIC_API_BASE_MODEL_TYPE_TEST_UTIL_H_
#define SYNC_INTERNAL_PUBLIC_API_BASE_MODEL_TYPE_TEST_UTIL_H_

#include <ostream>

#include "sync/internal_api/public/base/model_type.h"
#include "sync/notifier/object_id_invalidation_map.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace syncer {

ObjectIdInvalidationMap BuildInvalidationMap(
    ModelType type,
    int version,
    const std::string& payload);

void PrintTo(ModelTypeSet model_types, ::std::ostream* os);

::testing::Matcher<ModelTypeSet> HasModelTypes(ModelTypeSet expected_types);

}  

#endif  
