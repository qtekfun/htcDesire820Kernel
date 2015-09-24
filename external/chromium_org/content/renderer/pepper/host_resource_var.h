// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CONTENT_RENDERER_PEPPER_HOST_RESOURCE_VAR_H_
#define PPAPI_CONTENT_RENDERER_PEPPER_HOST_RESOURCE_VAR_H_

#include "base/memory/scoped_ptr.h"
#include "ipc/ipc_message.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/shared_impl/resource_var.h"
#include "ppapi/shared_impl/var.h"

namespace content {

class HostResourceVar : public ppapi::ResourceVar {
 public:
  
  HostResourceVar();

  
  explicit HostResourceVar(PP_Resource pp_resource);

  
  
  
  
  
  HostResourceVar(int pending_renderer_host_id,
                  const IPC::Message& creation_message);

  
  virtual PP_Resource GetPPResource() const OVERRIDE;
  virtual int GetPendingRendererHostId() const OVERRIDE;
  virtual int GetPendingBrowserHostId() const OVERRIDE;
  virtual const IPC::Message* GetCreationMessage() const OVERRIDE;
  virtual bool IsPending() const OVERRIDE;

  void set_pending_browser_host_id(int id) {
    pending_browser_host_id_ = id;
  }

 protected:
  virtual ~HostResourceVar();

 private:
  
  
  PP_Resource pp_resource_;

  
  int pending_renderer_host_id_;

  
  int pending_browser_host_id_;

  
  
  scoped_ptr<IPC::Message> creation_message_;

  DISALLOW_COPY_AND_ASSIGN(HostResourceVar);
};

}  

#endif
