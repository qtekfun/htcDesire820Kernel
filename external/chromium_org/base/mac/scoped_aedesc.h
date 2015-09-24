// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_SCOPED_AEDESC_H_
#define BASE_MAC_SCOPED_AEDESC_H_

#import <CoreServices/CoreServices.h>

#include "base/basictypes.h"

namespace base {
namespace mac {

template <typename AEDescType = AEDesc>
class ScopedAEDesc {
 public:
  ScopedAEDesc() {
    AECreateDesc(typeNull, NULL, 0, &desc_);
  }

  ~ScopedAEDesc() {
    AEDisposeDesc(&desc_);
  }

  
  operator const AEDescType*() {
    return &desc_;
  }

  
  AEDescType* OutPointer() {
    return &desc_;
  }

 private:
  AEDescType desc_;

  DISALLOW_COPY_AND_ASSIGN(ScopedAEDesc);
};

}  
}  

#endif  
