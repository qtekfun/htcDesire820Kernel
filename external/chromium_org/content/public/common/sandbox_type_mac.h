// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_SANDBOX_TYPE_MAC_H_
#define CONTENT_PUBLIC_COMMON_SANDBOX_TYPE_MAC_H_

namespace content {


enum SandboxType {
  SANDBOX_TYPE_FIRST_TYPE = 0,  

  SANDBOX_TYPE_RENDERER = SANDBOX_TYPE_FIRST_TYPE,

  
  
  
  
  SANDBOX_TYPE_WORKER,

  
  
  SANDBOX_TYPE_UTILITY,

  
  SANDBOX_TYPE_GPU,

  
  SANDBOX_TYPE_PPAPI,

  SANDBOX_TYPE_AFTER_LAST_TYPE,  
};

}  

#endif  
