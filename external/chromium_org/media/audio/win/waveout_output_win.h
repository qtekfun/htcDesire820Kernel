// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_WIN_WAVEOUT_OUTPUT_WIN_H_
#define MEDIA_AUDIO_WIN_WAVEOUT_OUTPUT_WIN_H_

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/win/scoped_handle.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"

namespace media {

class AudioManagerWin;

class PCMWaveOutAudioOutputStream : public AudioOutputStream {
 public:
  
  
  
  PCMWaveOutAudioOutputStream(AudioManagerWin* manager,
                              const AudioParameters& params,
                              int num_buffers,
                              UINT device_id);
  virtual ~PCMWaveOutAudioOutputStream();

  
  virtual bool Open();
  virtual void Close();
  virtual void Start(AudioSourceCallback* callback);
  virtual void Stop();
  virtual void SetVolume(double volume);
  virtual void GetVolume(double* volume);

  
  void QueueNextPacket(WAVEHDR* buffer);

 private:
  enum State {
    PCMA_BRAND_NEW,    
    PCMA_READY,        
    PCMA_PLAYING,      
    PCMA_STOPPING,     
    PCMA_CLOSED        
  };

  
  inline WAVEHDR* GetBuffer(int n) const;

  
  inline size_t BufferSize() const;

  
  
  static void NTAPI BufferCallback(PVOID lpParameter, BOOLEAN timer_fired);

  
  
  void HandleError(MMRESULT error);

  
  void SetupBuffers();

  
  void FreeBuffers();

  
  
  
  volatile State state_;

  
  
  AudioManagerWin* manager_;

  
  AudioSourceCallback* callback_;

  
  const int num_buffers_;

  
  uint32 buffer_size_;

  
  float volume_;

  
  const int channels_;

  
  uint32 pending_bytes_;

  
  
  UINT device_id_;

  
  WAVEFORMATPCMEX format_;

  
  HWAVEOUT waveout_;

  
  base::win::ScopedHandle buffer_event_;

  
  HANDLE waiting_handle_;

  
  
  scoped_ptr<char[]> buffers_;

  
  base::Lock lock_;

  
  scoped_ptr<AudioBus> audio_bus_;

  DISALLOW_COPY_AND_ASSIGN(PCMWaveOutAudioOutputStream);
};

}  

#endif  
