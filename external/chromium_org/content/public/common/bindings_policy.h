// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_BINDINGS_POLICY_H_
#define CONTENT_PUBLIC_COMMON_BINDINGS_POLICY_H_

namespace content {

enum BindingsPolicy {
  
  
  BINDINGS_POLICY_WEB_UI = 1 << 0,
  
  
  
  
  BINDINGS_POLICY_DOM_AUTOMATION = 1 << 1,
  
  BINDINGS_POLICY_EXTERNAL_HOST = 1 << 2,
  
  
  
  BINDINGS_POLICY_STATS_COLLECTION = 1 << 3,

};

}

#endif  
