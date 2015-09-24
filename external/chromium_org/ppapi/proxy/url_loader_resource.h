// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_URL_LOADER_RESOURCE_H_
#define PPAPI_PROXY_URL_LOADER_RESOURCE_H_

#include <deque>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ppapi/c/trusted/ppb_url_loader_trusted.h"
#include "ppapi/proxy/plugin_resource.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/shared_impl/url_request_info_data.h"
#include "ppapi/thunk/ppb_url_loader_api.h"

namespace ppapi {
namespace proxy {

class URLResponseInfoResource;

class PPAPI_PROXY_EXPORT URLLoaderResource
    : public PluginResource,
      public NON_EXPORTED_BASE(thunk::PPB_URLLoader_API) {
 public:
  
  URLLoaderResource(Connection connection,
                    PP_Instance instance);

  
  
  
  URLLoaderResource(Connection connection,
                    PP_Instance instance,
                    int pending_main_document_loader_id,
                    const URLResponseInfoData& data);

  virtual ~URLLoaderResource();

  
  thunk::PPB_URLLoader_API* AsPPB_URLLoader_API() OVERRIDE;

  
  virtual int32_t Open(PP_Resource request_id,
                       scoped_refptr<TrackedCallback> callback) OVERRIDE;
  virtual int32_t Open(const URLRequestInfoData& data,
                       int requestor_pid,
                       scoped_refptr<TrackedCallback> callback) OVERRIDE;
  virtual int32_t FollowRedirect(
      scoped_refptr<TrackedCallback> callback) OVERRIDE;
  virtual PP_Bool GetUploadProgress(int64_t* bytes_sent,
                                    int64_t* total_bytes_to_be_sent) OVERRIDE;
  virtual PP_Bool GetDownloadProgress(
      int64_t* bytes_received,
      int64_t* total_bytes_to_be_received) OVERRIDE;
  virtual PP_Resource GetResponseInfo() OVERRIDE;
  virtual int32_t ReadResponseBody(
      void* buffer,
      int32_t bytes_to_read,
      scoped_refptr<TrackedCallback> callback) OVERRIDE;
  virtual int32_t FinishStreamingToFile(
      scoped_refptr<TrackedCallback> callback) OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void GrantUniversalAccess() OVERRIDE;
  virtual void RegisterStatusCallback(
      PP_URLLoaderTrusted_StatusCallback callback) OVERRIDE;

  
  virtual void OnReplyReceived(const ResourceMessageReplyParams& params,
                               const IPC::Message& msg) OVERRIDE;

 private:
  enum Mode {
    
    MODE_WAITING_TO_OPEN,

    
    MODE_OPENING,

    
    MODE_STREAMING_DATA,

    
    MODE_LOAD_COMPLETE
  };

  
  void OnPluginMsgReceivedResponse(const ResourceMessageReplyParams& params,
                                   const URLResponseInfoData& data);
  void OnPluginMsgSendData(const ResourceMessageReplyParams& params,
                           const IPC::Message& message);
  void OnPluginMsgFinishedLoading(const ResourceMessageReplyParams& params,
                                  int32_t result);
  void OnPluginMsgUpdateProgress(const ResourceMessageReplyParams& params,
                                 int64_t bytes_sent,
                                 int64_t total_bytes_to_be_sent,
                                 int64_t bytes_received,
                                 int64_t total_bytes_to_be_received);

  
  
  void SetDefersLoading(bool defers_loading);

  int32_t ValidateCallback(scoped_refptr<TrackedCallback> callback);

  
  
  void RegisterCallback(scoped_refptr<TrackedCallback> callback);

  void RunCallback(int32_t result);

  
  
  void SaveResponseInfo(const URLResponseInfoData& data);

  size_t FillUserBuffer();

  Mode mode_;
  URLRequestInfoData request_data_;

  scoped_refptr<TrackedCallback> pending_callback_;

  PP_URLLoaderTrusted_StatusCallback status_callback_;

  std::deque<char> buffer_;
  int64_t bytes_sent_;
  int64_t total_bytes_to_be_sent_;
  int64_t bytes_received_;
  int64_t total_bytes_to_be_received_;
  char* user_buffer_;
  size_t user_buffer_size_;
  int32_t done_status_;
  bool is_streaming_to_file_;
  bool is_asynchronous_load_suspended_;

  
  scoped_refptr<URLResponseInfoResource> response_info_;

  DISALLOW_COPY_AND_ASSIGN(URLLoaderResource);
};

}  
}  

#endif  
