// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_PLUGIN_HOST_LOG_HANDLER_H_
#define REMOTING_HOST_PLUGIN_HOST_LOG_HANDLER_H_

#include <string>

namespace remoting {

class HostNPScriptObject;

class HostLogHandler {
 public:
  
  
  
  static void RegisterLogMessageHandler();

  
  
  

  static void RegisterLoggingScriptObject(HostNPScriptObject* script_object);
  static void UnregisterLoggingScriptObject(HostNPScriptObject* script_object);

 private:
  
  
  
  static bool LogToUI(int severity, const char* file, int line,
                      size_t message_start, const std::string& str);
};

}  

#endif  
