// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_JS_EVENT_ROUTER_H_
#define CHROME_BROWSER_SYNC_JS_EVENT_ROUTER_H_
#pragma once


#include <string>

namespace browser_sync {

class JsArgList;
class JsEventHandler;

class JsEventRouter {
 public:
  
  
  
  
  
  virtual void RouteJsEvent(const std::string& name, const JsArgList& args,
                            const JsEventHandler* target) = 0;

 protected:
  virtual ~JsEventRouter() {}
};

}  

#endif  
