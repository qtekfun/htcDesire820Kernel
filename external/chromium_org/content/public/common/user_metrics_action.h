// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_USER_METRICS_ACTION_H_
#define CONTENT_PUBLIC_COMMON_USER_METRICS_ACTION_H_

namespace content {

struct UserMetricsAction {
  const char* str_;
  explicit UserMetricsAction(const char* str) : str_(str) {}
};

}  

#endif  
