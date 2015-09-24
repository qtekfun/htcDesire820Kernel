// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_INPUT_INJECTOR_H_
#define REMOTING_HOST_INPUT_INJECTOR_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/protocol/clipboard_stub.h"
#include "remoting/protocol/input_stub.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace remoting {

class InputInjector : public protocol::ClipboardStub,
                      public protocol::InputStub {
 public:
  
  
  
  
  static scoped_ptr<InputInjector> Create(
      scoped_refptr<base::SingleThreadTaskRunner> main_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner);

  
  virtual void Start(
      scoped_ptr<protocol::ClipboardStub> client_clipboard) = 0;
};

}  

#endif  
