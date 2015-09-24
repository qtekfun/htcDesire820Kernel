// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_AUDIO_OUTPUT_IPC_H_
#define MEDIA_AUDIO_AUDIO_OUTPUT_IPC_H_

#include "base/memory/shared_memory.h"
#include "base/sync_socket.h"
#include "media/audio/audio_parameters.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT AudioOutputIPCDelegate {
 public:
  
  
  
  enum State {
    kPlaying,
    kPaused,
    kError
  };

  
  virtual void OnStateChanged(State state) = 0;

  
  
  
  
  
  // audio data to be written into the shared memory. The AudioOutputIPCDelegate
  
  
  virtual void OnStreamCreated(base::SharedMemoryHandle handle,
                               base::SyncSocket::Handle socket_handle,
                               int length) = 0;

  
  
  
  
  virtual void OnIPCClosed() = 0;

 protected:
  virtual ~AudioOutputIPCDelegate();
};

class MEDIA_EXPORT AudioOutputIPC {
 public:
  virtual ~AudioOutputIPC();

  
  
  
  
  
  
  virtual void CreateStream(AudioOutputIPCDelegate* delegate,
                            const AudioParameters& params,
                            int session_id) = 0;

  
  
  virtual void PlayStream() = 0;

  
  
  virtual void PauseStream() = 0;

  
  
  virtual void CloseStream() = 0;

  
  virtual void SetVolume(double volume) = 0;
};

}  

#endif  
