// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_UNRECOVERABLE_ERROR_HANDLER_H_
#define CHROME_BROWSER_SYNC_UNRECOVERABLE_ERROR_HANDLER_H_
#pragma once

#include <string>

#include "base/tracked.h"

namespace browser_sync {

class UnrecoverableErrorHandler {
 public:
  
  
  
  
  virtual void OnUnrecoverableError(const tracked_objects::Location& from_here,
                                    const std::string& message) = 0;
 protected:
  virtual ~UnrecoverableErrorHandler() { }
};

}

#endif  
