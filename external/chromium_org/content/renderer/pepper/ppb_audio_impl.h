// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PPB_AUDIO_IMPL_H_
#define CONTENT_RENDERER_PEPPER_PPB_AUDIO_IMPL_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/shared_memory.h"
#include "base/sync_socket.h"
#include "content/renderer/pepper/audio_helper.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/ppb_audio.h"
#include "ppapi/c/ppb_audio_config.h"
#include "ppapi/shared_impl/ppb_audio_config_shared.h"
#include "ppapi/shared_impl/ppb_audio_shared.h"
#include "ppapi/shared_impl/resource.h"
#include "ppapi/shared_impl/scoped_pp_resource.h"

namespace content {
class PepperPlatformAudioOutput;

class PPB_Audio_Impl : public ppapi::Resource,
                       public ppapi::PPB_Audio_Shared,
                       public AudioHelper {
 public:
  
  
  
  
  explicit PPB_Audio_Impl(PP_Instance instance);

  
  
  static PP_Resource Create(PP_Instance instance,
                            PP_Resource config_id,
                            const ppapi::AudioCallbackCombined& audio_callback,
                            void* user_data);

  
  bool Init(PP_Resource config_id,
            const ppapi::AudioCallbackCombined& user_callback,
            void* user_data);

  
  virtual ppapi::thunk::PPB_Audio_API* AsPPB_Audio_API() OVERRIDE;

  
  virtual PP_Resource GetCurrentConfig() OVERRIDE;
  virtual PP_Bool StartPlayback() OVERRIDE;
  virtual PP_Bool StopPlayback() OVERRIDE;
  virtual int32_t Open(
      PP_Resource config_id,
      scoped_refptr<ppapi::TrackedCallback> create_callback) OVERRIDE;
  virtual int32_t GetSyncSocket(int* sync_socket) OVERRIDE;
  virtual int32_t GetSharedMemory(int* shm_handle, uint32_t* shm_size) OVERRIDE;

 private:
  virtual ~PPB_Audio_Impl();

  
  virtual void OnSetStreamInfo(base::SharedMemoryHandle shared_memory_handle,
                               size_t shared_memory_size_,
                               base::SyncSocket::Handle socket) OVERRIDE;

  
  ppapi::ScopedPPResource config_;

  
  
  PepperPlatformAudioOutput* audio_;

  DISALLOW_COPY_AND_ASSIGN(PPB_Audio_Impl);
};

}  

#endif  
