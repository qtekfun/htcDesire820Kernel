// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_CLIENT_CONTEXT_H_
#define REMOTING_CLIENT_CLIENT_CONTEXT_H_

#include <string>

#include "base/threading/thread.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace remoting {

class ClientContext {
 public:
  
  
  ClientContext(base::SingleThreadTaskRunner* main_task_runner);
  virtual ~ClientContext();

  void Start();
  void Stop();

  base::SingleThreadTaskRunner* main_task_runner();
  base::SingleThreadTaskRunner* decode_task_runner();
  base::SingleThreadTaskRunner* audio_decode_task_runner();

 private:
  scoped_refptr<base::SingleThreadTaskRunner> main_task_runner_;

  
  base::Thread decode_thread_;

  
  base::Thread audio_decode_thread_;

  DISALLOW_COPY_AND_ASSIGN(ClientContext);
};

}  

#endif  
