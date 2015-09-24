// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_ENTER_PROXY_H_
#define PPAPI_PROXY_ENTER_PROXY_H_

#include "base/logging.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/proxy/host_dispatcher.h"
#include "ppapi/proxy/plugin_dispatcher.h"
#include "ppapi/proxy/plugin_globals.h"
#include "ppapi/proxy/plugin_resource_tracker.h"
#include "ppapi/thunk/enter.h"

namespace ppapi {

namespace proxy {

template<typename ResourceT>
class EnterPluginFromHostResource
    : public thunk::EnterResourceNoLock<ResourceT> {
 public:
  explicit EnterPluginFromHostResource(const HostResource& host_resource)
      : thunk::EnterResourceNoLock<ResourceT>(
            PluginGlobals::Get()->plugin_resource_tracker()->
                PluginResourceForHostResource(host_resource),
            false) {
    
    
    
    DCHECK(this->failed() ||
           PluginDispatcher::GetForInstance(host_resource.instance()));
  }
};

template<typename ResourceT>
class EnterHostFromHostResource
    : public thunk::EnterResourceNoLock<ResourceT> {
 public:
  explicit EnterHostFromHostResource(const HostResource& host_resource)
      : thunk::EnterResourceNoLock<ResourceT>(host_resource.host_resource(),
                                              false) {
    
    
    
    DCHECK(this->failed() ||
           HostDispatcher::GetForInstance(host_resource.instance()));
  }

  EnterHostFromHostResource(const HostResource& host_resource,
                            const pp::CompletionCallback& callback)
      : thunk::EnterResourceNoLock<ResourceT>(host_resource.host_resource(),
                                              callback.pp_completion_callback(),
                                              false) {
    
    
    
    DCHECK(this->failed() ||
           HostDispatcher::GetForInstance(host_resource.instance()));
  }
};

template<typename ResourceT>
class EnterHostFromHostResourceForceCallback
    : public EnterHostFromHostResource<ResourceT> {
 public:
  EnterHostFromHostResourceForceCallback(
      const HostResource& host_resource,
      const pp::CompletionCallback& callback)
      : EnterHostFromHostResource<ResourceT>(host_resource, callback),
        needs_running_(true) {
  }

  
  
  template<class CallbackFactory, typename Method>
  EnterHostFromHostResourceForceCallback(
      const HostResource& host_resource,
      CallbackFactory& factory,
      Method method)
      : EnterHostFromHostResource<ResourceT>(host_resource,
            factory.NewOptionalCallback(method)),
        needs_running_(true) {
    if (this->failed())
      RunCallback(PP_ERROR_BADRESOURCE);
  }

  
  template<class CallbackFactory, typename Method, typename A>
  EnterHostFromHostResourceForceCallback(
      const HostResource& host_resource,
      CallbackFactory& factory,
      Method method,
      const A& a)
      : EnterHostFromHostResource<ResourceT>(host_resource,
            factory.NewOptionalCallback(method, a)),
        needs_running_(true) {
    if (this->failed())
      RunCallback(PP_ERROR_BADRESOURCE);
  }

  
  template<class CallbackFactory, typename Method, typename A, typename B>
  EnterHostFromHostResourceForceCallback(
      const HostResource& host_resource,
      CallbackFactory& factory,
      Method method,
      const A& a,
      const B& b)
      : EnterHostFromHostResource<ResourceT>(host_resource,
            factory.NewOptionalCallback(method, a, b)),
        needs_running_(true) {
    if (this->failed())
      RunCallback(PP_ERROR_BADRESOURCE);
  }

  
  template<class CallbackFactory, typename Method, typename A, typename B,
           typename C>
  EnterHostFromHostResourceForceCallback(
      const HostResource& host_resource,
      CallbackFactory& factory,
      Method method,
      const A& a,
      const B& b,
      const C& c)
      : EnterHostFromHostResource<ResourceT>(host_resource,
            factory.NewOptionalCallback(method, a, b, c)),
        needs_running_(true) {
    if (this->failed())
      RunCallback(PP_ERROR_BADRESOURCE);
  }

  ~EnterHostFromHostResourceForceCallback() {
    if (needs_running_) {
      NOTREACHED() << "Should always call SetResult except in the "
                      "initialization failed case.";
      RunCallback(PP_ERROR_FAILED);
    }
  }

  void SetResult(int32_t result) {
    DCHECK(needs_running_) << "Don't call SetResult when there already is one.";
    if (result != PP_OK_COMPLETIONPENDING)
      RunCallback(result);
    needs_running_ = false;
    
    
    
    this->ClearCallback();
  }

 private:
  void RunCallback(int32_t result) {
    DCHECK(needs_running_);
    needs_running_ = false;
    this->callback()->Run(result);
    this->ClearCallback();
  }

  bool needs_running_;
};

}  
}  

#endif  
