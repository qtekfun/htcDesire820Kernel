// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_THREADING_POST_TASK_AND_REPLY_IMPL_H_
#define BASE_THREADING_POST_TASK_AND_REPLY_IMPL_H_

#include "base/callback_forward.h"
#include "base/location.h"

namespace base {
namespace internal {

class PostTaskAndReplyImpl {
 public:
  
  
  bool PostTaskAndReply(const tracked_objects::Location& from_here,
                        const Closure& task,
                        const Closure& reply);

 private:
  virtual bool PostTask(const tracked_objects::Location& from_here,
                        const Closure& task) = 0;
};

}  
}  

#endif  
