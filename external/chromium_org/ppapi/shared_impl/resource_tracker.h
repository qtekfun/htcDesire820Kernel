// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_RESOURCE_TRACKER_H_
#define PPAPI_SHARED_IMPL_RESOURCE_TRACKER_H_

#include <set>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"
#include "base/threading/thread_checker_impl.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

class Resource;

class PPAPI_SHARED_EXPORT ResourceTracker {
 public:
  
  
  
  
  enum ThreadMode { SINGLE_THREADED, THREAD_SAFE };
  explicit ResourceTracker(ThreadMode thread_mode);
  virtual ~ResourceTracker();

  
  
  Resource* GetResource(PP_Resource res) const;

  
  
  
  void AddRefResource(PP_Resource res);

  
  
  
  void ReleaseResource(PP_Resource res);

  
  void ReleaseResourceSoon(PP_Resource res);

  
  
  void DidCreateInstance(PP_Instance instance);

  
  
  
  void DidDeleteInstance(PP_Instance instance);

  
  
  int GetLiveObjectsForInstance(PP_Instance instance) const;

 protected:
  
  friend class Resource;

  
  
  void CheckThreadingPreconditions() const;

  
  
  
  
  
  
  void UseOddResourceValueInDebugMode();

  
  
  
  
  
  
  virtual PP_Resource AddResource(Resource* object);

  
  
  virtual void RemoveResource(Resource* object);

 private:
  
  
  void LastPluginRefWasDeleted(Resource* object);

  int32 GetNextResourceValue();

  
  bool CanOperateOnResource(PP_Resource res);

  typedef std::set<PP_Resource> ResourceSet;

  struct InstanceData {
    
    
    
    ResourceSet resources;
  };
  typedef base::hash_map<PP_Instance, linked_ptr<InstanceData> > InstanceMap;

  InstanceMap instance_map_;

  
  
  
  
  
  
  
  
  typedef std::pair<Resource*, int> ResourceAndRefCount;
  typedef base::hash_map<PP_Resource, ResourceAndRefCount> ResourceMap;
  ResourceMap live_resources_;

  int32 last_resource_value_;

  
  
  
  
  scoped_ptr<base::ThreadChecker> thread_checker_;

  base::WeakPtrFactory<ResourceTracker> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ResourceTracker);
};

}  

#endif  
