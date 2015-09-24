// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_TEST_MOCK_RESOURCE_CONTEXT_H_
#define CONTENT_PUBLIC_TEST_MOCK_RESOURCE_CONTEXT_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/resource_context.h"

namespace net {
class URLRequestContext;
}

namespace content {

class MockResourceContext : public ResourceContext {
 public:
  MockResourceContext();

  
  explicit MockResourceContext(net::URLRequestContext* test_request_context);

  virtual ~MockResourceContext();

  
  virtual net::HostResolver* GetHostResolver() OVERRIDE;
  virtual net::URLRequestContext* GetRequestContext() OVERRIDE;
  virtual bool AllowMicAccess(const GURL& origin) OVERRIDE;
  virtual bool AllowCameraAccess(const GURL& origin) OVERRIDE;

  
  
  void set_mic_access(bool mic_allowed) {
    mic_allowed_ = mic_allowed;
  }

  void set_camera_access(bool camera_allowed) {
    camera_allowed_ = camera_allowed;
  }

 private:
  net::URLRequestContext* test_request_context_;

  bool mic_allowed_;
  bool camera_allowed_;

  DISALLOW_COPY_AND_ASSIGN(MockResourceContext);
};

}  

#endif  
