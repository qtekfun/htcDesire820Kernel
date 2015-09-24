// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_JS_JS_EVENT_HANDLER_H_
#define SYNC_JS_JS_EVENT_HANDLER_H_


#include <string>

#include "sync/base/sync_export.h"

namespace syncer {

class JsEventDetails;

class SYNC_EXPORT JsEventHandler {
 public:
  virtual void HandleJsEvent(
      const std::string& name, const JsEventDetails& details) = 0;

 protected:
  virtual ~JsEventHandler() {}
};

}  

#endif  
