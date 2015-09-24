// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_PPAPI_GLOBALS_H_
#define PPAPI_SHARED_IMPL_PPAPI_GLOBALS_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/threading/thread_local.h"  
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/ppb_console.h"
#include "ppapi/shared_impl/api_id.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace base {
class MessageLoopProxy;
class TaskRunner;
}

namespace ppapi {

class CallbackTracker;
class MessageLoopShared;
class ResourceTracker;
class VarTracker;

namespace thunk {
class PPB_Instance_API;
class ResourceCreationAPI;
}

class PPAPI_SHARED_EXPORT PpapiGlobals {
 public:
  
  PpapiGlobals();

  
  
  
  
  struct PerThreadForTest {};
  explicit PpapiGlobals(PerThreadForTest);

  virtual ~PpapiGlobals();

  
  static PpapiGlobals* Get();

  
  
  
  
  
  
  
  
  
  static void SetPpapiGlobalsOnThreadForTest(PpapiGlobals* ptr);

  
  virtual ResourceTracker* GetResourceTracker() = 0;
  virtual VarTracker* GetVarTracker() = 0;
  virtual CallbackTracker* GetCallbackTrackerForInstance(
      PP_Instance instance) = 0;

  
  
  virtual void LogWithSource(PP_Instance instance,
                             PP_LogLevel level,
                             const std::string& source,
                             const std::string& value) = 0;

  
  
  
  
  
  
  
  
  virtual void BroadcastLogWithSource(PP_Module module,
                                      PP_LogLevel level,
                                      const std::string& source,
                                      const std::string& value) = 0;

  
  
  virtual thunk::PPB_Instance_API* GetInstanceAPI(PP_Instance instance) = 0;
  virtual thunk::ResourceCreationAPI* GetResourceCreationAPI(
      PP_Instance instance) = 0;

  
  
  virtual PP_Module GetModuleForInstance(PP_Instance instance) = 0;

  
  
  base::MessageLoopProxy* GetMainThreadMessageLoop();

  
  
  
  void ResetMainThreadMessageLoopForTesting();

  
  
  
  virtual MessageLoopShared* GetCurrentMessageLoop() = 0;

  
  
  
  virtual base::TaskRunner* GetFileTaskRunner() = 0;

  
  virtual std::string GetCmdLine() = 0;

  
  
  
  virtual void PreCacheFontForFlash(const void* logfontw) = 0;

  virtual bool IsHostGlobals() const;
  virtual bool IsPluginGlobals() const;

 private:
  
  
  
  static PpapiGlobals* GetThreadLocalPointer();

  scoped_refptr<base::MessageLoopProxy> main_loop_proxy_;

  DISALLOW_COPY_AND_ASSIGN(PpapiGlobals);
};

}  

#endif  
