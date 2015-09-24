// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NET_MODULE_H__
#define NET_BASE_NET_MODULE_H__

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT NetModule {
 public:
  typedef base::StringPiece (*ResourceProvider)(int key);

  
  static void SetResourceProvider(ResourceProvider func);

  
  
  
  static base::StringPiece GetResource(int key);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(NetModule);
};

}  

#endif  
