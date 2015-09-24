// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_USER_METRICS_H_
#define CONTENT_PUBLIC_BROWSER_USER_METRICS_H_

#include <string>

#include "base/callback.h"
#include "content/common/content_export.h"
#include "content/public/common/user_metrics_action.h"

namespace content {


CONTENT_EXPORT void RecordAction(const UserMetricsAction& action);

CONTENT_EXPORT void RecordComputedAction(const std::string& action);

typedef base::Callback<void(const std::string&)> ActionCallback;

CONTENT_EXPORT void AddActionCallback(const ActionCallback& callback);
CONTENT_EXPORT void RemoveActionCallback(const ActionCallback& callback);

}  

#endif  
