// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_REQUEST_PRIORITY_H_
#define NET_BASE_REQUEST_PRIORITY_H_

namespace net {

enum RequestPriority {
  IDLE = 0,
  MINIMUM_PRIORITY = IDLE,
  LOWEST,
  DEFAULT_PRIORITY = LOWEST,
  LOW,
  MEDIUM,
  HIGHEST,
  MAXIMUM_PRIORITY = HIGHEST,
};

enum RequestPrioritySize {
  NUM_PRIORITIES = MAXIMUM_PRIORITY + 1,
};

const char* RequestPriorityToString(RequestPriority priority);

}  

#endif  
