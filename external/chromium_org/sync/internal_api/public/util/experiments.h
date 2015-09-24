// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_UTIL_EXPERIMENTS_
#define SYNC_UTIL_EXPERIMENTS_

#include "sync/internal_api/public/base/model_type.h"

namespace syncer {

const char kAutofillCullingTag[] = "autofill_culling";
const char kFaviconSyncTag[] = "favicon_sync";
const char kPreCommitUpdateAvoidanceTag[] = "pre_commit_update_avoidance";

struct Experiments {
  Experiments() : autofill_culling(false),
                  favicon_sync_limit(200) {}

  bool Matches(const Experiments& rhs) {
    return (autofill_culling == rhs.autofill_culling &&
            favicon_sync_limit == rhs.favicon_sync_limit);
  }

  
  bool autofill_culling;

  
  int favicon_sync_limit;
};

}  

#endif  
