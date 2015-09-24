// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PPB_MESSAGE_LOOP_PROXY_H_
#define PPAPI_PROXY_PPB_MESSAGE_LOOP_PROXY_H_

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "ppapi/proxy/interface_proxy.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/shared_impl/ppb_message_loop_shared.h"
#include "ppapi/thunk/ppb_message_loop_api.h"

struct PPB_MessageLoop_1_0;

namespace ppapi {
namespace proxy {

class PPAPI_PROXY_EXPORT MessageLoopResource : public MessageLoopShared {
 public:
  explicit MessageLoopResource(PP_Instance instance);
  
  
  explicit MessageLoopResource(ForMainThread);
  virtual ~MessageLoopResource();

  
  virtual thunk::PPB_MessageLoop_API* AsPPB_MessageLoop_API() OVERRIDE;

  
  virtual int32_t AttachToCurrentThread() OVERRIDE;
  virtual int32_t Run() OVERRIDE;
  virtual int32_t PostWork(PP_CompletionCallback callback,
                           int64_t delay_ms) OVERRIDE;
  virtual int32_t PostQuit(PP_Bool should_destroy) OVERRIDE;

  static MessageLoopResource* GetCurrent();
  void DetachFromThread();
  bool is_main_thread_loop() const {
    return is_main_thread_loop_;
  }

 private:
  struct TaskInfo {
    tracked_objects::Location from_here;
    base::Closure closure;
    int64 delay_ms;
  };

  
  bool IsCurrent() const;

  
  
  
  
  
  virtual void PostClosure(const tracked_objects::Location& from_here,
                           const base::Closure& closure,
                           int64 delay_ms) OVERRIDE;

  virtual base::MessageLoopProxy* GetMessageLoopProxy() OVERRIDE;

  
  static void ReleaseMessageLoop(void* value);

  
  
  
  
  scoped_ptr<base::MessageLoop> loop_;
  scoped_refptr<base::MessageLoopProxy> loop_proxy_;

  
  int nested_invocations_;

  
  
  bool destroyed_;

  
  
  bool should_destroy_;

  bool is_main_thread_loop_;

  
  
  
  std::vector<TaskInfo> pending_tasks_;

  DISALLOW_COPY_AND_ASSIGN(MessageLoopResource);
};

class PPB_MessageLoop_Proxy : public InterfaceProxy {
 public:
  explicit PPB_MessageLoop_Proxy(Dispatcher* dispatcher);
  virtual ~PPB_MessageLoop_Proxy();

  static const PPB_MessageLoop_1_0* GetInterface();

 private:
  DISALLOW_COPY_AND_ASSIGN(PPB_MessageLoop_Proxy);
};

}  
}  

#endif  
