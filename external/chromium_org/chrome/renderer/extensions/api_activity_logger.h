// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_API_ACTIVITY_LOGGER_H_
#define CHROME_RENDERER_EXTENSIONS_API_ACTIVITY_LOGGER_H_

#include <string>
#include "chrome/renderer/extensions/chrome_v8_extension.h"
#include "chrome/renderer/extensions/dispatcher.h"
#include "extensions/common/features/feature.h"
#include "v8/include/v8.h"

namespace extensions {

class APIActivityLogger : public ChromeV8Extension {
 public:
  APIActivityLogger(Dispatcher* dispatcher, ChromeV8Context* context);

 private:
   
   enum CallType {
     APICALL,
     EVENT
   };

   
   
   
   
   
   static void LogAPICall(const v8::FunctionCallbackInfo<v8::Value>& args);

   
   
   
   
   
   static void LogEvent(const v8::FunctionCallbackInfo<v8::Value>& args);

   
   
   static void LogInternal(const CallType call_type,
                           const v8::FunctionCallbackInfo<v8::Value>& args);

  DISALLOW_COPY_AND_ASSIGN(APIActivityLogger);
};

}  

#endif  
