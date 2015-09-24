// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Copyright (c) 2011, Code Aurora Forum. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_REQUEST_PRIORITY_H__
#define NET_BASE_REQUEST_PRIORITY_H__
#pragma once

#include "webkit/glue/resource_type.h"

namespace net {

enum RequestPriority {
  HIGHEST = 0,   
  MEDIUM,
  LOW,
  LOWEST,
  IDLE,
  NUM_PRIORITIES,
};

RequestPriority DetermineRequestPriority(ResourceType::Type type) __attribute__((visibility ("default")));

}  

#endif  
