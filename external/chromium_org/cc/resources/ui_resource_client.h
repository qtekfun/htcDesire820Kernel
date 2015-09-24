// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_RESOURCES_UI_RESOURCE_CLIENT_H_
#define CC_RESOURCES_UI_RESOURCE_CLIENT_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "cc/base/cc_export.h"

namespace cc {

class UIResourceBitmap;

typedef int UIResourceId;

class CC_EXPORT UIResourceClient {
 public:
  
  
  
  
  
  
  virtual UIResourceBitmap GetBitmap(UIResourceId uid,
                                     bool resource_lost) = 0;
  virtual ~UIResourceClient() {}
};

}  

#endif  
