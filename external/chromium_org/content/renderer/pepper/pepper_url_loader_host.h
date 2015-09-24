// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PEPPER_URL_LOADER_HOST_H_
#define CONTENT_RENDERER_PEPPER_PEPPER_URL_LOADER_HOST_H_

#include <vector>

#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "content/common/content_export.h"
#include "ppapi/host/resource_host.h"
#include "ppapi/proxy/resource_message_params.h"
#include "ppapi/shared_impl/url_request_info_data.h"
#include "ppapi/shared_impl/url_response_info_data.h"
#include "third_party/WebKit/public/platform/WebURLLoaderClient.h"

namespace blink {
class WebFrame;
class WebURLLoader;
}

namespace content {

class RendererPpapiHostImpl;

class PepperURLLoaderHost
    : public ppapi::host::ResourceHost,
      public blink::WebURLLoaderClient {
 public:
  
  
  PepperURLLoaderHost(RendererPpapiHostImpl* host,
                      bool main_document_loader,
                      PP_Instance instance,
                      PP_Resource resource);
  virtual ~PepperURLLoaderHost();

  
  virtual int32_t OnResourceMessageReceived(
      const IPC::Message& msg,
      ppapi::host::HostMessageContext* context) OVERRIDE;

  
  virtual void willSendRequest(blink::WebURLLoader* loader,
                               blink::WebURLRequest& new_request,
                               const blink::WebURLResponse& redir_response);
  virtual void didSendData(blink::WebURLLoader* loader,
                           unsigned long long bytes_sent,
                           unsigned long long total_bytes_to_be_sent);
  virtual void didReceiveResponse(blink::WebURLLoader* loader,
                                  const blink::WebURLResponse& response);
  virtual void didDownloadData(blink::WebURLLoader* loader,
                               int data_length,
                               int encoded_data_length);
  virtual void didReceiveData(blink::WebURLLoader* loader,
                              const char* data,
                              int data_length,
                              int encoded_data_length);
  virtual void didFinishLoading(blink::WebURLLoader* loader,
                                double finish_time);
  virtual void didFail(blink::WebURLLoader* loader,
                       const blink::WebURLError& error);

 private:
  
  virtual void DidConnectPendingHostToResource() OVERRIDE;

  
  int32_t OnHostMsgOpen(ppapi::host::HostMessageContext* context,
                        const ppapi::URLRequestInfoData& request_data);
  int32_t InternalOnHostMsgOpen(ppapi::host::HostMessageContext* context,
                                const ppapi::URLRequestInfoData& request_data);
  int32_t OnHostMsgSetDeferLoading(ppapi::host::HostMessageContext* context,
                                   bool defers_loading);
  int32_t OnHostMsgClose(ppapi::host::HostMessageContext* context);
  int32_t OnHostMsgGrantUniversalAccess(
      ppapi::host::HostMessageContext* context);

  
  
  
  
  
  
  void SendUpdateToPlugin(IPC::Message* msg);

  
  
  
  
  
  void SendOrderedUpdateToPlugin(IPC::Message* msg);

  void Close();

  
  blink::WebFrame* GetFrame();

  
  
  void SetDefersLoading(bool defers_loading);

  
  void SaveResponse(const blink::WebURLResponse& response);
  void DidDataFromWebURLResponse(const ppapi::URLResponseInfoData& data);

  
  void UpdateProgress();

  
  RendererPpapiHostImpl* renderer_ppapi_host_;

  
  
  bool main_document_loader_;

  
  ppapi::URLRequestInfoData request_data_;

  
  bool has_universal_access_;

  
  
  
  
  
  
  
  scoped_ptr<blink::WebURLLoader> loader_;

  int64_t bytes_sent_;
  int64_t total_bytes_to_be_sent_;
  int64_t bytes_received_;
  int64_t total_bytes_to_be_received_;

  
  
  
  ScopedVector<IPC::Message> pending_replies_;
  ScopedVector<IPC::Message> out_of_order_replies_;

  
  
  
  bool pending_response_;

  base::WeakPtrFactory<PepperURLLoaderHost> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PepperURLLoaderHost);
};

}  

#endif  
