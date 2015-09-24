// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_AUDIO_INPUT_IPC_H_
#define MEDIA_AUDIO_AUDIO_INPUT_IPC_H_

#include "base/memory/shared_memory.h"
#include "base/sync_socket.h"
#include "media/audio/audio_parameters.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT AudioInputIPCDelegate {
 public:
  
  enum State {
    kRecording,
    kStopped,
    kError
  };

  
  
  
  
  
  
  
  
  virtual void OnStreamCreated(base::SharedMemoryHandle handle,
                               base::SyncSocket::Handle socket_handle,
                               int length,
                               int total_segments) = 0;

  
  virtual void OnStateChanged(State state) = 0;

  
  virtual void OnVolume(double volume) = 0;

  
  
  
  
  virtual void OnIPCClosed() = 0;

 protected:
  virtual ~AudioInputIPCDelegate();
};

class MEDIA_EXPORT AudioInputIPC {
 public:
  virtual ~AudioInputIPC();

  
  
  
  
  
  virtual void CreateStream(AudioInputIPCDelegate* delegate,
                            int session_id,
                            const AudioParameters& params,
                            bool automatic_gain_control,
                            uint32 total_segments) = 0;

  
  virtual void RecordStream() = 0;

  
  virtual void SetVolume(double volume) = 0;

  
  
  virtual void CloseStream() = 0;
};

}  

#endif  
