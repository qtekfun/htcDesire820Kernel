// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_PROTOCOL_CLIPBOARD_THREAD_PROXY_H_
#define REMOTING_PROTOCOL_CLIPBOARD_THREAD_PROXY_H_

#include "base/memory/weak_ptr.h"
#include "base/task_runner.h"
#include "remoting/protocol/clipboard_stub.h"

namespace base {
class MessageLoopProxy;
}  

namespace remoting {
namespace protocol {

class ClipboardThreadProxy : public ClipboardStub {
 public:
  virtual ~ClipboardThreadProxy();

  
  
  ClipboardThreadProxy(
      const base::WeakPtr<ClipboardStub>& clipboard_stub,
      scoped_refptr<base::TaskRunner> clipboard_stub_task_runner);

  
  virtual void InjectClipboardEvent(const ClipboardEvent& event) OVERRIDE;

 private:
  
  
  static void InjectClipboardEventStatic(
      const base::WeakPtr<ClipboardStub>& clipboard_stub,
      const ClipboardEvent& event);

  base::WeakPtr<ClipboardStub> clipboard_stub_;
  scoped_refptr<base::TaskRunner> clipboard_stub_task_runner_;

  DISALLOW_COPY_AND_ASSIGN(ClipboardThreadProxy);
};

}  
}  

#endif  
