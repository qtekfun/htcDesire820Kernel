// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_NPAPI_NPOBJECT_BASE_H_
#define CONTENT_CHILD_NPAPI_NPOBJECT_BASE_H_

#include "ipc/ipc_listener.h"
#include "third_party/npapi/bindings/npruntime.h"

struct NPObject;

namespace content {

class NPObjectBase {
 public:
  virtual ~NPObjectBase() {}

  
  virtual NPObject* GetUnderlyingNPObject() = 0;

  
  virtual IPC::Listener* GetChannelListener() = 0;
};

}  

#endif  
