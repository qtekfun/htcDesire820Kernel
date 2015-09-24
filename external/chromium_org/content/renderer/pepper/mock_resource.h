// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_MOCK_RESOURCE_H_
#define CONTENT_RENDERER_PEPPER_MOCK_RESOURCE_H_

#include "ppapi/shared_impl/resource.h"

namespace content {

class MockResource : public ppapi::Resource {
 public:
  MockResource(PP_Instance instance)
      : Resource(ppapi::OBJECT_IS_IMPL, instance) {}

 private:
  virtual ~MockResource() {}
};

}  

#endif  
