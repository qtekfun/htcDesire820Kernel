// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_AUDIO_INPUT_RESOURCE_H_
#define PPAPI_PROXY_AUDIO_INPUT_RESOURCE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/sync_socket.h"
#include "base/threading/simple_thread.h"
#include "ppapi/proxy/device_enumeration_resource_helper.h"
#include "ppapi/proxy/plugin_resource.h"
#include "ppapi/shared_impl/scoped_pp_resource.h"
#include "ppapi/thunk/ppb_audio_input_api.h"

namespace ppapi {
namespace proxy {

class ResourceMessageReplyParams;

class AudioInputResource : public PluginResource,
                           public thunk::PPB_AudioInput_API,
                           public base::DelegateSimpleThread::Delegate {
 public:
  AudioInputResource(Connection connection, PP_Instance instance);
  virtual ~AudioInputResource();

  
  virtual thunk::PPB_AudioInput_API* AsPPB_AudioInput_API() OVERRIDE;
  virtual void OnReplyReceived(const ResourceMessageReplyParams& params,
                               const IPC::Message& msg) OVERRIDE;

  
  virtual int32_t EnumerateDevices0_2(
      PP_Resource* devices,
      scoped_refptr<TrackedCallback> callback) OVERRIDE;
  virtual int32_t EnumerateDevices(
      const PP_ArrayOutput& output,
      scoped_refptr<TrackedCallback> callback) OVERRIDE;
  virtual int32_t MonitorDeviceChange(
      PP_MonitorDeviceChangeCallback callback,
      void* user_data) OVERRIDE;
  virtual int32_t Open0_2(PP_Resource device_ref,
                          PP_Resource config,
                          PPB_AudioInput_Callback_0_2 audio_input_callback_0_2,
                          void* user_data,
                          scoped_refptr<TrackedCallback> callback) OVERRIDE;
  virtual int32_t Open(PP_Resource device_ref,
                       PP_Resource config,
                       PPB_AudioInput_Callback audio_input_callback,
                       void* user_data,
                       scoped_refptr<TrackedCallback> callback) OVERRIDE;
  virtual PP_Resource GetCurrentConfig() OVERRIDE;
  virtual PP_Bool StartCapture() OVERRIDE;
  virtual PP_Bool StopCapture() OVERRIDE;
  virtual void Close() OVERRIDE;

 protected:
  
  virtual void LastPluginRefWasDeleted() OVERRIDE;

 private:
  enum OpenState {
    BEFORE_OPEN,
    OPENED,
    CLOSED
  };

  void OnPluginMsgOpenReply(const ResourceMessageReplyParams& params);

  
  
  void SetStreamInfo(base::SharedMemoryHandle shared_memory_handle,
                     size_t shared_memory_size,
                     base::SyncSocket::Handle socket_handle);

  
  void StartThread();

  
  void StopThread();

  
  
  virtual void Run() OVERRIDE;

  int32_t CommonOpen(PP_Resource device_ref,
                     PP_Resource config,
                     PPB_AudioInput_Callback_0_2 audio_input_callback_0_2,
                     PPB_AudioInput_Callback audio_input_callback,
                     void* user_data,
                     scoped_refptr<TrackedCallback> callback);

  OpenState open_state_;

  
  bool capturing_;

  
  
  scoped_ptr<base::CancelableSyncSocket> socket_;

  
  
  
  scoped_ptr<base::SharedMemory> shared_memory_;

  
  size_t shared_memory_size_;

  
  scoped_ptr<base::DelegateSimpleThread> audio_input_thread_;

  
  PPB_AudioInput_Callback_0_2 audio_input_callback_0_2_;
  PPB_AudioInput_Callback audio_input_callback_;

  
  void* user_data_;

  
  
  scoped_refptr<TrackedCallback> open_callback_;

  
  
  ScopedPPResource config_;

  DeviceEnumerationResourceHelper enumeration_helper_;

  
  
  size_t bytes_per_second_;

  DISALLOW_COPY_AND_ASSIGN(AudioInputResource);
};

}  
}  

#endif  
