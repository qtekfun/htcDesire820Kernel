// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_JS_BACKEND_H_
#define CHROME_BROWSER_SYNC_JS_BACKEND_H_
#pragma once


#include <string>

namespace browser_sync {

class JsArgList;
class JsEventHandler;
class JsEventRouter;

class JsBackend {
 public:
  
  
  virtual void SetParentJsEventRouter(JsEventRouter* router) = 0;

  
  virtual void RemoveParentJsEventRouter() = 0;

  
  
  virtual const JsEventRouter* GetParentJsEventRouter() const = 0;

  
  
  virtual void ProcessMessage(
      const std::string& name, const JsArgList& args,
      const JsEventHandler* sender) = 0;

 protected:
  virtual ~JsBackend() {}
};

}  

#endif  
