// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_UTIL_UNRECOVERABLE_ERROR_HANDLER_H_
#define SYNC_UTIL_UNRECOVERABLE_ERROR_HANDLER_H_

#include <string>

#include "base/location.h"

namespace syncer {

class UnrecoverableErrorHandler {
 public:
  
  
  
  
  virtual void OnUnrecoverableError(const tracked_objects::Location& from_here,
                                    const std::string& message) = 0;
  virtual ~UnrecoverableErrorHandler() {}
};

}  

#endif  
