// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_SEND_REQUEST_NATIVES_H_
#define CHROME_RENDERER_EXTENSIONS_SEND_REQUEST_NATIVES_H_

#include "chrome/renderer/extensions/chrome_v8_extension.h"
#include "chrome/renderer/extensions/dispatcher.h"

#include "v8/include/v8.h"

namespace extensions {
class ChromeV8Context;
class RequestSender;

class SendRequestNatives : public ChromeV8Extension {
 public:
  SendRequestNatives(Dispatcher* dispatcher,
                     RequestSender* request_sender,
                     ChromeV8Context* context);

 private:
  void GetNextRequestId(const v8::FunctionCallbackInfo<v8::Value>& args);

  
  
  void StartRequest(const v8::FunctionCallbackInfo<v8::Value>& args);

  
  void GetGlobal(const v8::FunctionCallbackInfo<v8::Value>& args);

  RequestSender* request_sender_;

  DISALLOW_COPY_AND_ASSIGN(SendRequestNatives);
};

}  

#endif  
