// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_AUDIO_IO_H_
#define MEDIA_AUDIO_AUDIO_IO_H_

#include "base/basictypes.h"
#include "media/audio/audio_buffers_state.h"
#include "media/base/audio_bus.h"



// as the data is written to the audio device. Size of each packet is determined

namespace media {

class MEDIA_EXPORT AudioOutputStream {
 public:
  
  
  
  
  class MEDIA_EXPORT AudioSourceCallback {
   public:
    
    
    
    virtual int OnMoreData(AudioBus* dest,
                           AudioBuffersState buffers_state) = 0;

    virtual int OnMoreIOData(AudioBus* source,
                             AudioBus* dest,
                             AudioBuffersState buffers_state) = 0;

    
    
    
    
    virtual void OnError(AudioOutputStream* stream) = 0;

   protected:
    virtual ~AudioSourceCallback() {}
  };

  virtual ~AudioOutputStream() {}

  
  
  virtual bool Open() = 0;

  
  
  
  
  
  virtual void Start(AudioSourceCallback* callback) = 0;

  
  
  virtual void Stop() = 0;

  
  virtual void SetVolume(double volume) = 0;

  
  virtual void GetVolume(double* volume) = 0;

  
  
  virtual void Close() = 0;
};

class MEDIA_EXPORT AudioInputStream {
 public:
  class MEDIA_EXPORT AudioInputCallback {
   public:
    
    
    
    virtual void OnData(AudioInputStream* stream, const uint8* src,
                        uint32 size, uint32 hardware_delay_bytes,
                        double volume) = 0;

    
    
    virtual void OnClose(AudioInputStream* stream) = 0;

    
    
    
    
    virtual void OnError(AudioInputStream* stream) = 0;

   protected:
    virtual ~AudioInputCallback() {}
  };

  virtual ~AudioInputStream() {}

  
  
  virtual bool Open() = 0;

  
  
  virtual void Start(AudioInputCallback* callback) = 0;

  
  
  
  virtual void Stop() = 0;

  
  
  virtual void Close() = 0;

  
  
  virtual double GetMaxVolume() = 0;

  
  virtual void SetVolume(double volume) = 0;

  
  virtual double GetVolume() = 0;

  
  virtual void SetAutomaticGainControl(bool enabled) = 0;

  
  virtual bool GetAutomaticGainControl() = 0;
};

}  

#endif  
