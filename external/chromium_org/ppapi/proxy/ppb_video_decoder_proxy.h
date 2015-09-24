// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PPB_VIDEO_DECODER_PROXY_H_
#define PPAPI_PROXY_PPB_VIDEO_DECODER_PROXY_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/proxy/interface_proxy.h"
#include "ppapi/proxy/proxy_completion_callback_factory.h"
#include "ppapi/shared_impl/ppb_video_decoder_shared.h"
#include "ppapi/thunk/ppb_video_decoder_api.h"
#include "ppapi/utility/completion_callback_factory.h"

namespace ppapi {
namespace proxy {

class PPB_VideoDecoder_Proxy : public InterfaceProxy {
 public:
  PPB_VideoDecoder_Proxy(Dispatcher* dispatcher);
  virtual ~PPB_VideoDecoder_Proxy();

  
  static PP_Resource CreateProxyResource(
      PP_Instance instance,
      PP_Resource graphics_context,
      PP_VideoDecoder_Profile profile);

  
  virtual bool OnMessageReceived(const IPC::Message& msg);

  static const ApiID kApiID = API_ID_PPB_VIDEO_DECODER_DEV;

 private:
  
  
  void OnMsgCreate(PP_Instance instance,
                   const ppapi::HostResource& graphics_context,
                   PP_VideoDecoder_Profile profile,
                   ppapi::HostResource* result);
  void OnMsgDecode(
      const ppapi::HostResource& decoder,
      const ppapi::HostResource& buffer, int32 id, uint32 size);
  void OnMsgAssignPictureBuffers(
      const ppapi::HostResource& decoder,
      const std::vector<PP_PictureBuffer_Dev>& buffers);
  void OnMsgReusePictureBuffer(
      const ppapi::HostResource& decoder,
      int32 picture_buffer_id);
  void OnMsgFlush(const ppapi::HostResource& decoder);
  void OnMsgReset(const ppapi::HostResource& decoder);
  void OnMsgDestroy(const ppapi::HostResource& decoder);

  
  
  void SendMsgEndOfBitstreamACKToPlugin(
      int32_t result, const ppapi::HostResource& decoder, int32 id);
  void SendMsgFlushACKToPlugin(
      int32_t result, const ppapi::HostResource& decoder);
  void SendMsgResetACKToPlugin(
      int32_t result, const ppapi::HostResource& decoder);

  
  
  void OnMsgEndOfBitstreamACK(const ppapi::HostResource& decoder,
                              int32_t id, int32_t result);
  void OnMsgFlushACK(const ppapi::HostResource& decoder, int32_t result);
  void OnMsgResetACK(const ppapi::HostResource& decoder, int32_t result);

  ProxyCompletionCallbackFactory<PPB_VideoDecoder_Proxy> callback_factory_;

  DISALLOW_COPY_AND_ASSIGN(PPB_VideoDecoder_Proxy);
};

}  
}  

#endif  
