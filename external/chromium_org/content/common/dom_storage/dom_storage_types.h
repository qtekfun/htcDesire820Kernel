// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_DOM_STORAGE_DOM_STORAGE_TYPES_H_
#define CONTENT_COMMON_DOM_STORAGE_DOM_STORAGE_TYPES_H_

#include <map>

#include "base/basictypes.h"
#include "base/strings/nullable_string16.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "url/gurl.h"

namespace content {

typedef std::map<base::string16, base::NullableString16> DOMStorageValuesMap;

const size_t kPerStorageAreaQuota = 10 * 1024 * 1024;

const size_t kPerStorageAreaOverQuotaAllowance = 100 * 1024;

const int64 kLocalStorageNamespaceId = 0;

const int64 kInvalidSessionStorageNamespaceId = kLocalStorageNamespaceId;

const int64 kMaxInMemoryStorageAreas = 100;

}  

#endif  
