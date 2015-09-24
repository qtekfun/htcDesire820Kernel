// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_HOST_RESOURCE_HOST_H_
#define PPAPI_HOST_RESOURCE_HOST_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/host/ppapi_host_export.h"
#include "ppapi/host/resource_message_handler.h"
#include "ppapi/shared_impl/host_resource.h"

namespace IPC {
class Message;
}

namespace ppapi {
namespace host {

struct HostMessageContext;
class PpapiHost;
class ResourceMessageFilter;

class PPAPI_HOST_EXPORT ResourceHost : public ResourceMessageHandler {
 public:
  ResourceHost(PpapiHost* host, PP_Instance instance, PP_Resource resource);
  virtual ~ResourceHost();

  PpapiHost* host() { return host_; }
  PP_Instance pp_instance() const { return pp_instance_; }
  PP_Resource pp_resource() const { return pp_resource_; }

  
  
  
  virtual bool HandleMessage(const IPC::Message& msg,
                             HostMessageContext* context) OVERRIDE;

  
  
  
  
  
  
  void SetPPResourceForPendingHost(PP_Resource pp_resource);

  virtual void SendReply(const ReplyMessageContext& context,
                         const IPC::Message& msg) OVERRIDE;

  
  
  virtual bool IsFileRefHost();
  virtual bool IsFileSystemHost();
  virtual bool IsGraphics2DHost();

 protected:
  
  
  
  
  void AddFilter(scoped_refptr<ResourceMessageFilter> filter);

  
  
  
  
  
  virtual void DidConnectPendingHostToResource() {}

 private:
  
  PpapiHost* host_;

  PP_Instance pp_instance_;
  PP_Resource pp_resource_;

  
  
  std::vector<scoped_refptr<ResourceMessageFilter> > message_filters_;

  DISALLOW_COPY_AND_ASSIGN(ResourceHost);
};

}  
}  

#endif  
