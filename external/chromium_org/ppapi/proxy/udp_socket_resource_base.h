// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_UDP_SOCKET_RESOURCE_BASE_H_
#define PPAPI_PROXY_UDP_SOCKET_RESOURCE_BASE_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "ppapi/c/ppb_udp_socket.h"
#include "ppapi/c/private/ppb_net_address_private.h"
#include "ppapi/proxy/plugin_resource.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/shared_impl/tracked_callback.h"

namespace ppapi {
namespace proxy {

class ResourceMessageReplyParams;

class PPAPI_PROXY_EXPORT UDPSocketResourceBase: public PluginResource {
 public:
  
  
  static const int32_t kMaxReadSize;
  
  
  static const int32_t kMaxWriteSize;

  
  
  
  
  static const int32_t kMaxSendBufferSize;
  
  
  
  
  static const int32_t kMaxReceiveBufferSize;

 protected:
  UDPSocketResourceBase(Connection connection,
                        PP_Instance instance,
                        bool private_api);
  virtual ~UDPSocketResourceBase();

  int32_t SetOptionImpl(PP_UDPSocket_Option name,
                        const PP_Var& value,
                        scoped_refptr<TrackedCallback> callback);
  int32_t BindImpl(const PP_NetAddress_Private* addr,
                   scoped_refptr<TrackedCallback> callback);
  PP_Bool GetBoundAddressImpl(PP_NetAddress_Private* addr);
  
  int32_t RecvFromImpl(char* buffer,
                       int32_t num_bytes,
                       PP_Resource* addr,
                       scoped_refptr<TrackedCallback> callback);
  PP_Bool GetRecvFromAddressImpl(PP_NetAddress_Private* addr);
  int32_t SendToImpl(const char* buffer,
                     int32_t num_bytes,
                     const PP_NetAddress_Private* addr,
                     scoped_refptr<TrackedCallback> callback);
  void CloseImpl();

 private:
  void PostAbortIfNecessary(scoped_refptr<TrackedCallback>* callback);

  
  void OnPluginMsgSetOptionReply(scoped_refptr<TrackedCallback> callback,
                                 const ResourceMessageReplyParams& params);
  void OnPluginMsgBindReply(const ResourceMessageReplyParams& params,
                            const PP_NetAddress_Private& bound_addr);
  void OnPluginMsgRecvFromReply(PP_Resource* output_addr,
                                const ResourceMessageReplyParams& params,
                                const std::string& data,
                                const PP_NetAddress_Private& addr);
  void OnPluginMsgSendToReply(const ResourceMessageReplyParams& params,
                              int32_t bytes_written);

  void RunCallback(scoped_refptr<TrackedCallback> callback, int32_t pp_result);

  bool private_api_;
  bool bound_;
  bool closed_;

  scoped_refptr<TrackedCallback> bind_callback_;
  scoped_refptr<TrackedCallback> recvfrom_callback_;
  scoped_refptr<TrackedCallback> sendto_callback_;

  char* read_buffer_;
  int32_t bytes_to_read_;

  PP_NetAddress_Private recvfrom_addr_;
  PP_NetAddress_Private bound_addr_;

  DISALLOW_COPY_AND_ASSIGN(UDPSocketResourceBase);
};

}  
}  

#endif  
