// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_AUDIO_OUTPUT_DEVICE_H_
#define MEDIA_AUDIO_AUDIO_OUTPUT_DEVICE_H_

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/message_loop/message_loop.h"
#include "media/audio/audio_device_thread.h"
#include "media/audio/audio_output_ipc.h"
#include "media/audio/audio_parameters.h"
#include "media/audio/scoped_loop_observer.h"
#include "media/base/audio_renderer_sink.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT AudioOutputDevice
    : NON_EXPORTED_BASE(public AudioRendererSink),
      NON_EXPORTED_BASE(public AudioOutputIPCDelegate),
      NON_EXPORTED_BASE(public ScopedLoopObserver) {
 public:
  
  AudioOutputDevice(scoped_ptr<AudioOutputIPC> ipc,
                    const scoped_refptr<base::MessageLoopProxy>& io_loop);

  
  
  
  
  
  
  
  
  void InitializeUnifiedStream(const AudioParameters& params,
                               RenderCallback* callback,
                               int session_id);

  
  virtual void Initialize(const AudioParameters& params,
                          RenderCallback* callback) OVERRIDE;
  virtual void Start() OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Play() OVERRIDE;
  virtual void Pause() OVERRIDE;
  virtual bool SetVolume(double volume) OVERRIDE;

  
  
  virtual void OnStateChanged(AudioOutputIPCDelegate::State state) OVERRIDE;
  virtual void OnStreamCreated(base::SharedMemoryHandle handle,
                               base::SyncSocket::Handle socket_handle,
                               int length) OVERRIDE;
  virtual void OnIPCClosed() OVERRIDE;

 protected:
  
  
  friend class base::RefCountedThreadSafe<AudioOutputDevice>;
  virtual ~AudioOutputDevice();

 private:
  
  enum State {
    IPC_CLOSED,  
    IDLE,  
    CREATING_STREAM,  
    PAUSED,  
    PLAYING,  
  };

  
  
  
  
  void CreateStreamOnIOThread(const AudioParameters& params);
  void PlayOnIOThread();
  void PauseOnIOThread();
  void ShutDownOnIOThread();
  void SetVolumeOnIOThread(double volume);

  
  
  virtual void WillDestroyCurrentMessageLoop() OVERRIDE;

  AudioParameters audio_parameters_;

  RenderCallback* callback_;

  
  
  
  scoped_ptr<AudioOutputIPC> ipc_;

  
  
  State state_;

  
  bool play_on_start_;

  
  
  int session_id_;

  
  class AudioThreadCallback;

  
  
  base::Lock audio_thread_lock_;
  AudioDeviceThread audio_thread_;
  scoped_ptr<AudioOutputDevice::AudioThreadCallback> audio_callback_;

  
  
  
  
  
  
  bool stopping_hack_;

  DISALLOW_COPY_AND_ASSIGN(AudioOutputDevice);
};

}  

#endif  
