// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_THUNK_ENTER_H_
#define PPAPI_THUNK_ENTER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/shared_impl/ppapi_globals.h"
#include "ppapi/shared_impl/proxy_lock.h"
#include "ppapi/shared_impl/resource.h"
#include "ppapi/shared_impl/resource_tracker.h"
#include "ppapi/shared_impl/singleton_resource_id.h"
#include "ppapi/shared_impl/tracked_callback.h"
#include "ppapi/thunk/ppapi_thunk_export.h"
#include "ppapi/thunk/ppb_instance_api.h"
#include "ppapi/thunk/resource_creation_api.h"

namespace ppapi {
namespace thunk {


namespace subtle {

template <bool lock_on_entry>
struct LockOnEntry;

template <>
struct LockOnEntry<false> {
#if (!NDEBUG)
  LockOnEntry() {
    
    ProxyLock::AssertAcquired();
  }
  ~LockOnEntry() {
    
    
    ProxyLock::AssertAcquired();
  }
#endif
};

template <>
struct LockOnEntry<true> {
  LockOnEntry() {
    ppapi::ProxyLock::Acquire();
  }
  ~LockOnEntry() {
    ppapi::ProxyLock::Release();
  }
};

class PPAPI_THUNK_EXPORT EnterBase {
 public:
  EnterBase();
  explicit EnterBase(PP_Resource resource);
  EnterBase(PP_Instance instance, SingletonResourceID resource_id);
  EnterBase(PP_Resource resource, const PP_CompletionCallback& callback);
  EnterBase(PP_Instance instance, SingletonResourceID resource_id,
            const PP_CompletionCallback& callback);
  virtual ~EnterBase();

  
  
  
  
  
  
  
  
  
  int32_t SetResult(int32_t result);

  
  int32_t retval() const { return retval_; }

  
  
  bool succeeded() const { return retval_ == PP_OK; }
  bool failed() const { return !succeeded(); }

  const scoped_refptr<TrackedCallback>& callback() { return callback_; }

 protected:
  
  
  
  static Resource* GetResource(PP_Resource resource);

  
  
  static Resource* GetSingletonResource(PP_Instance instance,
                                        SingletonResourceID resource_id);

  void ClearCallback();

  
  
  
  
  
  
  
  
  
  void SetStateForResourceError(PP_Resource pp_resource,
                                Resource* resource_base,
                                void* object,
                                bool report_error);

  
  void SetStateForFunctionError(PP_Instance pp_instance,
                                void* object,
                                bool report_error);

  
  
  
  Resource* resource_;

 private:
  bool CallbackIsValid() const;

  
  
  
  
  void SetStateForCallbackError(bool report_error);

  
  
  scoped_refptr<TrackedCallback> callback_;

  int32_t retval_;
};

}  


template<typename ResourceT, bool lock_on_entry = true>
class EnterResource
    : public subtle::LockOnEntry<lock_on_entry>,  
      public subtle::EnterBase {
 public:
  EnterResource(PP_Resource resource, bool report_error)
      : EnterBase(resource) {
    Init(resource, report_error);
  }
  EnterResource(PP_Resource resource, const PP_CompletionCallback& callback,
                bool report_error)
      : EnterBase(resource, callback) {
    Init(resource, report_error);
  }
  ~EnterResource() {}

  ResourceT* object() { return object_; }
  Resource* resource() { return resource_; }

 private:
  void Init(PP_Resource resource, bool report_error) {
    if (resource_)
      object_ = resource_->GetAs<ResourceT>();
    else
      object_ = NULL;
    
    
    SetStateForResourceError(resource, resource_, object_, report_error);
  }

  ResourceT* object_;

  DISALLOW_COPY_AND_ASSIGN(EnterResource);
};


template<typename ResourceT>
class EnterResourceNoLock : public EnterResource<ResourceT, false> {
 public:
  EnterResourceNoLock(PP_Resource resource, bool report_error)
      : EnterResource<ResourceT, false>(resource, report_error) {
  }
  EnterResourceNoLock(PP_Resource resource,
                      const PP_CompletionCallback& callback,
                      bool report_error)
      : EnterResource<ResourceT, false>(resource, callback, report_error) {
  }
};


class PPAPI_THUNK_EXPORT EnterInstance
    : public subtle::LockOnEntry<true>,  
      public subtle::EnterBase {
 public:
  explicit EnterInstance(PP_Instance instance);
  EnterInstance(PP_Instance instance,
                const PP_CompletionCallback& callback);
  ~EnterInstance();

  bool succeeded() const { return !!functions_; }
  bool failed() const { return !functions_; }

  PPB_Instance_API* functions() const { return functions_; }

 private:
  PPB_Instance_API* functions_;
};

class PPAPI_THUNK_EXPORT EnterInstanceNoLock
    : public subtle::LockOnEntry<false>,  
      public subtle::EnterBase {
 public:
  explicit EnterInstanceNoLock(PP_Instance instance);
  EnterInstanceNoLock(PP_Instance instance,
                      const PP_CompletionCallback& callback);
  ~EnterInstanceNoLock();

  PPB_Instance_API* functions() { return functions_; }

 private:
  PPB_Instance_API* functions_;
};


template<typename ApiT, bool lock_on_entry = true>
class EnterInstanceAPI
    : public subtle::LockOnEntry<lock_on_entry>,  
      public subtle::EnterBase {
 public:
  explicit EnterInstanceAPI(PP_Instance instance)
      : EnterBase(instance, ApiT::kSingletonResourceID),
        functions_(NULL) {
    if (resource_)
      functions_ = resource_->GetAs<ApiT>();
    SetStateForFunctionError(instance, functions_, true);
  }
  EnterInstanceAPI(PP_Instance instance,
                   const PP_CompletionCallback& callback)
      : EnterBase(instance, ApiT::kSingletonResourceID, callback),
        functions_(NULL) {
    if (resource_)
      functions_ = resource_->GetAs<ApiT>();
    SetStateForFunctionError(instance, functions_, true);
  }
  ~EnterInstanceAPI() {}

  bool succeeded() const { return !!functions_; }
  bool failed() const { return !functions_; }

  ApiT* functions() const { return functions_; }

 private:
  ApiT* functions_;
};

template<typename ApiT>
class EnterInstanceAPINoLock : public EnterInstanceAPI<ApiT, false> {
 public:
  explicit EnterInstanceAPINoLock(PP_Instance instance)
      : EnterInstanceAPI<ApiT, false>(instance) {
  }
};


class PPAPI_THUNK_EXPORT EnterResourceCreation
    : public subtle::LockOnEntry<true>,  
      public subtle::EnterBase {
 public:
  explicit EnterResourceCreation(PP_Instance instance);
  ~EnterResourceCreation();

  ResourceCreationAPI* functions() { return functions_; }

 private:
  ResourceCreationAPI* functions_;
};

class PPAPI_THUNK_EXPORT EnterResourceCreationNoLock
    : public subtle::LockOnEntry<false>,  
      public subtle::EnterBase {
 public:
  explicit EnterResourceCreationNoLock(PP_Instance instance);
  ~EnterResourceCreationNoLock();

  ResourceCreationAPI* functions() { return functions_; }

 private:
  ResourceCreationAPI* functions_;
};

}  
}  

#endif  
