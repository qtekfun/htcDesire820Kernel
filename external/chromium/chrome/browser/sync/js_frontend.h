// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_JS_FRONTEND_H_
#define CHROME_BROWSER_SYNC_JS_FRONTEND_H_
#pragma once


#include <string>

namespace browser_sync {

class JsArgList;
class JsEventHandler;

class JsFrontend {
 public:
  
  
  
  virtual void AddHandler(JsEventHandler* handler) = 0;

  
  
  virtual void RemoveHandler(JsEventHandler* handler) = 0;

  
  
  virtual void ProcessMessage(
      const std::string& name, const JsArgList& args,
      const JsEventHandler* sender) = 0;

 protected:
  virtual ~JsFrontend() {}
};

}  

#endif  
