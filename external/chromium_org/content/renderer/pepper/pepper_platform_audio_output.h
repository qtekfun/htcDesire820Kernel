// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PEPPER_PLATFORM_AUDIO_OUTPUT_IMPL_H_
#define CONTENT_RENDERER_PEPPER_PEPPER_PLATFORM_AUDIO_OUTPUT_IMPL_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "media/audio/audio_output_ipc.h"

namespace media {
class AudioParameters;
}

namespace base {
class MessageLoopProxy;
}

namespace content {
class AudioHelper;

class PepperPlatformAudioOutput
    : public media::AudioOutputIPCDelegate,
      public base::RefCountedThreadSafe<PepperPlatformAudioOutput> {
 public:
  
  
  static PepperPlatformAudioOutput* Create(int sample_rate,
                                           int frames_per_buffer,
                                           int source_render_view_id,
                                           AudioHelper* client);

  

  
  
  bool StartPlayback();

  
  
  bool StopPlayback();

  
  
  void ShutDown();

  
  virtual void OnStateChanged(
      media::AudioOutputIPCDelegate::State state) OVERRIDE;
  virtual void OnStreamCreated(base::SharedMemoryHandle handle,
                               base::SyncSocket::Handle socket_handle,
                               int length) OVERRIDE;
  virtual void OnIPCClosed() OVERRIDE;

 protected:
  virtual ~PepperPlatformAudioOutput();

 private:
  friend class base::RefCountedThreadSafe<PepperPlatformAudioOutput>;

  PepperPlatformAudioOutput();

  bool Initialize(int sample_rate,
                  int frames_per_buffer,
                  int source_render_view_id,
                  AudioHelper* client);

  
  void InitializeOnIOThread(const media::AudioParameters& params);
  void StartPlaybackOnIOThread();
  void StopPlaybackOnIOThread();
  void ShutDownOnIOThread();

  
  
  AudioHelper* client_;

  
  
  scoped_ptr<media::AudioOutputIPC> ipc_;

  scoped_refptr<base::MessageLoopProxy> main_message_loop_proxy_;
  scoped_refptr<base::MessageLoopProxy> io_message_loop_proxy_;

  DISALLOW_COPY_AND_ASSIGN(PepperPlatformAudioOutput);
};

}  

#endif  
