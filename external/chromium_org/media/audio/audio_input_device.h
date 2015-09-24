// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_AUDIO_AUDIO_INPUT_DEVICE_H_
#define MEDIA_AUDIO_AUDIO_INPUT_DEVICE_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "media/audio/audio_device_thread.h"
#include "media/audio/audio_input_ipc.h"
#include "media/audio/audio_parameters.h"
#include "media/audio/scoped_loop_observer.h"
#include "media/base/audio_capturer_source.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT AudioInputDevice
    : NON_EXPORTED_BASE(public AudioCapturerSource),
      NON_EXPORTED_BASE(public AudioInputIPCDelegate),
      NON_EXPORTED_BASE(public ScopedLoopObserver) {
 public:
  
  AudioInputDevice(scoped_ptr<AudioInputIPC> ipc,
                   const scoped_refptr<base::MessageLoopProxy>& io_loop);

  
  virtual void Initialize(const AudioParameters& params,
                          CaptureCallback* callback,
                          int session_id) OVERRIDE;
  virtual void Start() OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual void SetAutomaticGainControl(bool enabled) OVERRIDE;

 protected:
  friend class base::RefCountedThreadSafe<AudioInputDevice>;
  virtual ~AudioInputDevice();

  
  
  virtual void OnStreamCreated(base::SharedMemoryHandle handle,
                               base::SyncSocket::Handle socket_handle,
                               int length,
                               int total_segments) OVERRIDE;
  virtual void OnVolume(double volume) OVERRIDE;
  virtual void OnStateChanged(
      AudioInputIPCDelegate::State state) OVERRIDE;
  virtual void OnIPCClosed() OVERRIDE;

 private:
  
  enum State {
    IPC_CLOSED,  
    IDLE,  
    CREATING_STREAM,  
    RECORDING,  
  };

  
  
  
  
  void StartUpOnIOThread();
  void ShutDownOnIOThread();
  void SetVolumeOnIOThread(double volume);
  void SetAutomaticGainControlOnIOThread(bool enabled);

  
  
  virtual void WillDestroyCurrentMessageLoop() OVERRIDE;

  AudioParameters audio_parameters_;

  CaptureCallback* callback_;

  
  
  
  scoped_ptr<AudioInputIPC> ipc_;

  
  
  State state_;

  
  
  int session_id_;

  
  
  bool agc_is_enabled_;

  
  class AudioThreadCallback;

  
  
  base::Lock audio_thread_lock_;
  AudioDeviceThread audio_thread_;
  scoped_ptr<AudioInputDevice::AudioThreadCallback> audio_callback_;

  
  
  
  
  
  
  bool stopping_hack_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(AudioInputDevice);
};

}  

#endif  
