// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_CONTEXT_MENU_CLIENT_H_
#define CONTENT_PUBLIC_RENDERER_CONTEXT_MENU_CLIENT_H_

#include "base/basictypes.h"
#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT ContextMenuClient {
 public:
  
  
  
  virtual void OnMenuAction(int request_id, unsigned action) = 0;

  
  
  virtual void OnMenuClosed(int request_id) = 0;

 protected:
  virtual ~ContextMenuClient() {}
};

}  

#endif  
