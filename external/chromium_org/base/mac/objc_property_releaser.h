// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_OBJC_PROPERTY_RELEASER_H_
#define BASE_MAC_OBJC_PROPERTY_RELEASER_H_

#import <Foundation/Foundation.h>

#include "base/base_export.h"

namespace base {
namespace mac {


class BASE_EXPORT ObjCPropertyReleaser {
 public:
  
  
  
  

  
  
  ~ObjCPropertyReleaser() {
    ReleaseProperties();
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void Init(id object, Class classy);

  
  
  
  void ReleaseProperties();

 private:
  id object_;
  Class class_;
};

}  
}  

#endif  
