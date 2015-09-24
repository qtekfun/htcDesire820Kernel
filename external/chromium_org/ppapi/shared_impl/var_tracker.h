// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_VAR_TRACKER_H_
#define PPAPI_SHARED_IMPL_VAR_TRACKER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/threading/thread_checker.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/shared_impl/host_resource.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"
#include "ppapi/shared_impl/var.h"

namespace IPC {
class Message;
}  

namespace ppapi {

class ArrayBufferVar;

class PPAPI_SHARED_EXPORT VarTracker {
 public:
  
  
  
  
  enum ThreadMode { SINGLE_THREADED, THREAD_SAFE };
  explicit VarTracker(ThreadMode thread_mode);
  virtual ~VarTracker();

  
  int32 AddVar(Var* var);

  
  
  
  Var* GetVar(int32 var_id) const;
  Var* GetVar(const PP_Var& var) const;

  
  
  
  bool AddRefVar(int32 var_id);
  bool AddRefVar(const PP_Var& var);

  
  
  
  
  bool ReleaseVar(int32 var_id);
  bool ReleaseVar(const PP_Var& var);

  
  
  PP_Var MakeArrayBufferPPVar(uint32 size_in_bytes);
  
  PP_Var MakeArrayBufferPPVar(uint32 size_in_bytes, const void* data);
  
  
  PP_Var MakeArrayBufferPPVar(uint32 size_in_bytes,
                              base::SharedMemoryHandle h);

  
  
  
  
  ArrayBufferVar* MakeArrayBufferVar(uint32 size_in_bytes, const void* data);

  
  
  
  
  virtual PP_Var MakeResourcePPVarFromMessage(
      PP_Instance instance,
      const IPC::Message& creation_message,
      int pending_renderer_id,
      int pending_browser_id) = 0;

  
  
  
  
  
  PP_Var MakeResourcePPVar(PP_Resource pp_resource);

  
  
  
  
  
  
  virtual ResourceVar* MakeResourceVar(PP_Resource pp_resource) = 0;

  
  
  
  
  std::vector<PP_Var> GetLiveVars();

  
  
  
  int GetRefCountForObject(const PP_Var& object);
  int GetTrackedWithNoReferenceCountForObject(const PP_Var& object);

  
  
  static bool IsVarTypeRefcounted(PP_VarType type);

  
  virtual void DidDeleteInstance(PP_Instance instance) = 0;

  
  
  
  virtual int TrackSharedMemoryHandle(PP_Instance instance,
                                      base::SharedMemoryHandle handle,
                                      uint32 size_in_bytes) = 0;

  
  
  
  virtual bool StopTrackingSharedMemoryHandle(
      int id,
      PP_Instance instance,
      base::SharedMemoryHandle *handle,
      uint32* size_in_bytes) = 0;

 protected:
  struct PPAPI_SHARED_EXPORT VarInfo {
    VarInfo();
    VarInfo(Var* v, int input_ref_count);

    scoped_refptr<Var> var;

    
    
    
    int ref_count;

    
    
    
    
    
    
    
    
    
    int track_with_no_reference_count;
  };
  typedef base::hash_map<int32, VarInfo> VarMap;

  
  enum AddVarRefMode {
    ADD_VAR_TAKE_ONE_REFERENCE,
    ADD_VAR_CREATE_WITH_NO_REFERENCE
  };

  
  
  void CheckThreadingPreconditions() const;

  
  
  
  
  virtual int32 AddVarInternal(Var* var, AddVarRefMode mode);

  
  VarMap::iterator GetLiveVar(int32 id);
  VarMap::iterator GetLiveVar(const PP_Var& var);
  VarMap::const_iterator GetLiveVar(const PP_Var& var) const;

  
  
  
  
  virtual void TrackedObjectGettingOneRef(VarMap::const_iterator iter);

  
  
  
  
  
  virtual void ObjectGettingZeroRef(VarMap::iterator iter);

  
  
  
  
  
  
  virtual bool DeleteObjectInfoIfNecessary(VarMap::iterator iter);

  VarMap live_vars_;

  
  int32 last_var_id_;

 private:
  
  
  
  virtual ArrayBufferVar* CreateArrayBuffer(uint32 size_in_bytes) = 0;
  virtual ArrayBufferVar* CreateShmArrayBuffer(
      uint32 size_in_bytes,
      base::SharedMemoryHandle handle) = 0;

  
  
  
  
  scoped_ptr<base::ThreadChecker> thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(VarTracker);
};

}  

#endif  
