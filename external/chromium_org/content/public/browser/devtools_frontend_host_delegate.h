// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DEVTOOLS_FRONTEND_HOST_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_DEVTOOLS_FRONTEND_HOST_DELEGATE_H_

#include <string>

namespace content {

class DevToolsFrontendHostDelegate {
 public:
  virtual ~DevToolsFrontendHostDelegate() {}

  
  virtual void DispatchOnEmbedder(const std::string& message) = 0;

  
  
  virtual void InspectedContentsClosing() = 0;
};

}  

#endif  
