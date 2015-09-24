// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_PPB_AUDIO_SHARED_H_
#define PPAPI_SHARED_IMPL_PPB_AUDIO_SHARED_H_

#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/sync_socket.h"
#include "base/threading/simple_thread.h"
#include "media/base/audio_bus.h"
#include "ppapi/c/ppb_audio.h"
#include "ppapi/c/ppb_audio_config.h"
#include "ppapi/shared_impl/resource.h"
#include "ppapi/thunk/ppb_audio_api.h"

#if defined(OS_NACL)
#include "native_client/src/untrusted/irt/irt_ppapi.h"
#endif

namespace ppapi {

class PPAPI_SHARED_EXPORT AudioCallbackCombined {
 public:
  AudioCallbackCombined();
  explicit AudioCallbackCombined(PPB_Audio_Callback_1_0 callback_1_0);
  explicit AudioCallbackCombined(PPB_Audio_Callback callback);

  ~AudioCallbackCombined();

  bool IsValid() const;

  void Run(void* sample_buffer,
           uint32_t buffer_size_in_bytes,
           PP_TimeDelta latency,
           void* user_data) const;

 private:
  PPB_Audio_Callback_1_0 callback_1_0_;
  PPB_Audio_Callback callback_;
};

class PPAPI_SHARED_EXPORT PPB_Audio_Shared
    : public thunk::PPB_Audio_API,
      public base::DelegateSimpleThread::Delegate {
 public:
  PPB_Audio_Shared();
  virtual ~PPB_Audio_Shared();

  bool playing() const { return playing_; }

  
  
  
  
  void SetCallback(const AudioCallbackCombined& callback, void* user_data);

  
  
  
  
  
  
  
  
  void SetStartPlaybackState();
  void SetStopPlaybackState();

  
  
  void SetStreamInfo(PP_Instance instance,
                     base::SharedMemoryHandle shared_memory_handle,
                     size_t shared_memory_size,
                     base::SyncSocket::Handle socket_handle,
                     PP_AudioSampleRate sample_rate,
                     int sample_frame_count);

#if defined(OS_NACL)
  
  
  static void SetThreadFunctions(const struct PP_ThreadFunctions* functions);
#endif

 private:
  
  void StartThread();

  
  void StopThread();

  
  virtual void Run();

  
  bool playing_;

  
  
  scoped_ptr<base::CancelableSyncSocket> socket_;

  
  
  
  scoped_ptr<base::SharedMemory> shared_memory_;

  
  size_t shared_memory_size_;

#if !defined(OS_NACL)
  
  scoped_ptr<base::DelegateSimpleThread> audio_thread_;
#else
  uintptr_t thread_id_;
  bool thread_active_;

  static void CallRun(void* self);
#endif

  
  AudioCallbackCombined callback_;

  
  void* user_data_;

  
  scoped_ptr<media::AudioBus> audio_bus_;

  
  int client_buffer_size_bytes_;
  scoped_ptr<uint8_t[]> client_buffer_;

  
  size_t bytes_per_second_;

  
  uint32_t buffer_index_;

  DISALLOW_COPY_AND_ASSIGN(PPB_Audio_Shared);
};

}  

#endif  
