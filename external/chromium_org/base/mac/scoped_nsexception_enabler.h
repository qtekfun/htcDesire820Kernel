// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_SCOPED_NSEXCEPTION_ENABLER_H_
#define BASE_MAC_SCOPED_NSEXCEPTION_ENABLER_H_

#import <Foundation/Foundation.h>

#include "base/base_export.h"
#include "base/basictypes.h"

namespace base {
namespace mac {


class BASE_EXPORT ScopedNSExceptionEnabler {
 public:
  ScopedNSExceptionEnabler();
  ~ScopedNSExceptionEnabler();

 private:
  bool was_enabled_;

  DISALLOW_COPY_AND_ASSIGN(ScopedNSExceptionEnabler);
};

BASE_EXPORT bool GetNSExceptionsAllowed();
BASE_EXPORT void SetNSExceptionsAllowed(bool allowed);

typedef id (^BlockReturningId)();
BASE_EXPORT id RunBlockIgnoringExceptions(BlockReturningId block);

}  
}  

#endif  
