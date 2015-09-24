// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_NETWORK_PROXY_HOST_H_
#define CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_NETWORK_PROXY_HOST_H_

#include <queue>
#include <string>

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "content/common/content_export.h"
#include "net/proxy/proxy_service.h"
#include "ppapi/host/host_message_context.h"
#include "ppapi/host/resource_host.h"

namespace net {
class ProxyInfo;
class URLRequestContextGetter;
}

namespace ppapi {
namespace host {
struct ReplyMessageContext;
}
}

namespace content {

class BrowserPpapiHostImpl;

class CONTENT_EXPORT PepperNetworkProxyHost : public ppapi::host::ResourceHost {
 public:
  PepperNetworkProxyHost(BrowserPpapiHostImpl* host,
                         PP_Instance instance,
                         PP_Resource resource);

  virtual ~PepperNetworkProxyHost();

 private:
  
  
  
  struct UIThreadData {
    UIThreadData();
    ~UIThreadData();
    bool is_allowed;
    scoped_refptr<net::URLRequestContextGetter> context_getter;
  };
  static UIThreadData GetUIThreadDataOnUIThread(int render_process_id,
                                                int render_view_id,
                                                bool is_external_plugin);
  void DidGetUIThreadData(const UIThreadData&);

  
  virtual int32_t OnResourceMessageReceived(
      const IPC::Message& msg,
      ppapi::host::HostMessageContext* context) OVERRIDE;

  int32_t OnMsgGetProxyForURL(ppapi::host::HostMessageContext* context,
                              const std::string& url);

  
  void TryToSendUnsentRequests();

  void OnResolveProxyCompleted(ppapi::host::ReplyMessageContext context,
                               net::ProxyInfo* proxy_info,
                               int result);
  void SendFailureReply(int32_t error,
                        ppapi::host::ReplyMessageContext context);

  
  
  
  net::ProxyService* proxy_service_;
  bool is_allowed_;

  
  
  bool waiting_for_ui_thread_data_;

  
  
  
  struct UnsentRequest {
    GURL url;
    ppapi::host::ReplyMessageContext reply_context;
  };
  std::queue<UnsentRequest> unsent_requests_;

  
  
  std::queue<net::ProxyService::PacRequest*> pending_requests_;

  base::WeakPtrFactory<PepperNetworkProxyHost> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PepperNetworkProxyHost);
};

}  

#endif  
