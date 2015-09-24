// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_BINDINGS_LIB_MESSAGE_QUEUE_H_
#define MOJO_PUBLIC_BINDINGS_LIB_MESSAGE_QUEUE_H_

#include <queue>

#include "mojo/public/system/macros.h"

namespace mojo {

class Message;

class MessageQueue {
 public:
  MessageQueue();
  ~MessageQueue();

  bool IsEmpty() const;
  Message* Peek();

  
  
  void Push(Message* message);

  
  
  void Pop(Message* message);

  
  
  void Pop();

 private:
  std::queue<Message*> queue_;

  MOJO_DISALLOW_COPY_AND_ASSIGN(MessageQueue);
};

}  

#endif  
