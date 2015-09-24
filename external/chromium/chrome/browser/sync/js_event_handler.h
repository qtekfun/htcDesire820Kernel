// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_JS_EVENT_HANDLER_H_
#define CHROME_BROWSER_SYNC_JS_EVENT_HANDLER_H_
#pragma once


#include <string>

namespace browser_sync {

class JsArgList;

class JsEventHandler {
 public:
  virtual void HandleJsEvent(
      const std::string& name, const JsArgList& args) = 0;

 protected:
  virtual ~JsEventHandler() {}
};

}  

#endif  
