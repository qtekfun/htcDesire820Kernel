// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_LOOP_PROXY_H_
#define BASE_MESSAGE_LOOP_MESSAGE_LOOP_PROXY_H_

#include "base/base_export.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/single_thread_task_runner.h"

namespace base {

class BASE_EXPORT MessageLoopProxy : public SingleThreadTaskRunner {
 public:
  
  
  static scoped_refptr<MessageLoopProxy> current();

 protected:
  MessageLoopProxy();
  virtual ~MessageLoopProxy();
};

}  

#endif  
