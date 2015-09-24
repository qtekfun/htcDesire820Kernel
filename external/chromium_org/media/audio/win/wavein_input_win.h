// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_WIN_WAVEIN_INPUT_WIN_H_
#define MEDIA_AUDIO_WIN_WAVEIN_INPUT_WIN_H_

#include <string>

#include <windows.h>
#include <mmsystem.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "base/win/scoped_handle.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"

namespace media {

class AudioManagerWin;

class PCMWaveInAudioInputStream : public AudioInputStream {
 public:
  
  
  
  PCMWaveInAudioInputStream(AudioManagerWin* manager,
                            const AudioParameters& params,
                            int num_buffers,
                            const std::string& device_id);
  virtual ~PCMWaveInAudioInputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Start(AudioInputCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Close() OVERRIDE;
  
  virtual double GetMaxVolume() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual double GetVolume() OVERRIDE;
  virtual void SetAutomaticGainControl(bool enabled) OVERRIDE;
  virtual bool GetAutomaticGainControl() OVERRIDE;

 private:
  enum State {
    kStateEmpty,      
    kStateReady,      
    kStateRecording,  
    kStateStopping,   
    kStateStopped,    
    kStateClosed      
  };

  
  friend class AudioManagerTest;

  
  
  static void CALLBACK WaveCallback(HWAVEIN hwi, UINT msg, DWORD_PTR instance,
                                    DWORD_PTR param1, DWORD_PTR param2);

  
  
  void HandleError(MMRESULT error);

  
  void SetupBuffers();

  
  void FreeBuffers();

  
  void QueueNextPacket(WAVEHDR* buffer);

  
  
  bool GetDeviceId(UINT* device_index);

  base::ThreadChecker thread_checker_;

  
  
  
  volatile State state_;

  
  
  AudioManagerWin* manager_;

  
  AudioInputCallback* callback_;

  
  const int num_buffers_;

  
  uint32 buffer_size_;

  
  const int channels_;

  
  
  
  std::string device_id_;

  
  WAVEFORMATEX format_;

  
  HWAVEIN wavein_;

  
  WAVEHDR* buffer_;

  
  base::win::ScopedHandle stopped_event_;

  
  base::Lock lock_;

  DISALLOW_COPY_AND_ASSIGN(PCMWaveInAudioInputStream);
};

}  

#endif  
