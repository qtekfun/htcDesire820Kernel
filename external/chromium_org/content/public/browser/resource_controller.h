// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_RESOURCE_CONTROLLER_H_
#define CONTENT_PUBLIC_BROWSER_RESOURCE_CONTROLLER_H_

#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT ResourceController {
 public:
  virtual void Cancel() = 0;
  virtual void CancelAndIgnore() = 0;
  virtual void CancelWithError(int error_code) = 0;
  virtual void Resume() = 0;
 protected:
  virtual ~ResourceController() {}
};

}  

#endif  
