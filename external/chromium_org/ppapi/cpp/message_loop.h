// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_MESSAGE_LOOP_H_
#define PPAPI_CPP_MESSAGE_LOOP_H_

#include "ppapi/cpp/resource.h"


namespace pp {

class CompletionCallback;
class InstanceHandle;

class MessageLoop : public Resource {
 public:
  
  MessageLoop();

  
  
  
  
  
  
  explicit MessageLoop(const InstanceHandle& instance);

  MessageLoop(const MessageLoop& other);

  
  explicit MessageLoop(PP_Resource pp_message_loop);

  static MessageLoop GetForMainThread();
  static MessageLoop GetCurrent();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t AttachToCurrentThread();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Run();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t PostWork(const CompletionCallback& callback,
                   int64_t delay_ms = 0);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t PostQuit(bool should_destroy);
};

}  

#endif  
