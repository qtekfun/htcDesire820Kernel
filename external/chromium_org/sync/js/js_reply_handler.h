// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_JS_JS_REPLY_HANDLER_H_
#define SYNC_JS_JS_REPLY_HANDLER_H_


#include <string>

namespace syncer {

class JsArgList;

class JsReplyHandler {
 public:
  virtual void HandleJsReply(
      const std::string& name, const JsArgList& args) = 0;

 protected:
  virtual ~JsReplyHandler() {}
};

}  

#endif  
