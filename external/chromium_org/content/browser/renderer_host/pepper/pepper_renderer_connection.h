// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_RENDERER_CONNECTION_H_
#define CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_RENDERER_CONNECTION_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/browser_message_filter.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"

class GURL;

namespace ppapi {
namespace proxy {
class ResourceMessageCallParams;
}
}

namespace content {

class BrowserPpapiHostImpl;
struct PepperRendererInstanceData;

class PepperRendererConnection : public BrowserMessageFilter {
 public:
  explicit PepperRendererConnection(int render_process_id);

  
  virtual bool OnMessageReceived(const IPC::Message& msg,
                                 bool* message_was_ok) OVERRIDE;

 private:
  virtual ~PepperRendererConnection();

  
  
  
  BrowserPpapiHostImpl* GetHostForChildProcess(int child_process_id) const;

  void OnMsgCreateResourceHostsFromHost(
      int routing_id,
      int child_process_id,
      const ppapi::proxy::ResourceMessageCallParams& params,
      PP_Instance instance,
      const std::vector<IPC::Message>& nested_msgs);

  void OnMsgDidCreateInProcessInstance(
      PP_Instance instance,
      const PepperRendererInstanceData& instance_data);
  void OnMsgDidDeleteInProcessInstance(PP_Instance instance);

  int render_process_id_;

  
  
  
  
  scoped_ptr<BrowserPpapiHostImpl> in_process_host_;

  DISALLOW_COPY_AND_ASSIGN(PepperRendererConnection);
};

}  

#endif  
