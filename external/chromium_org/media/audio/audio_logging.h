// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_AUDIO_LOGGING_H_
#define MEDIA_AUDIO_AUDIO_LOGGING_H_

#include <string>

#include "base/memory/scoped_ptr.h"

namespace media {
class AudioParameters;

class AudioLog {
 public:
  virtual ~AudioLog() {}

  
  
  
  
  virtual void OnCreated(int component_id,
                         const media::AudioParameters& params,
                         const std::string& input_device_id,
                         const std::string& output_device_id) = 0;

  
  
  virtual void OnStarted(int component_id) = 0;

  
  
  virtual void OnStopped(int component_id) = 0;

  
  
  virtual void OnClosed(int component_id) = 0;

  
  virtual void OnError(int component_id) = 0;

  
  virtual void OnSetVolume(int component_id, double volume) = 0;
};

class AudioLogFactory {
 public:
  enum AudioComponent {
    
    
    AUDIO_INPUT_CONTROLLER,
    
    
    AUDIO_OUTPUT_CONTROLLER,
    AUDIO_OUTPUT_STREAM,
    AUDIO_COMPONENT_MAX
  };

  
  
  
  virtual scoped_ptr<AudioLog> CreateAudioLog(AudioComponent component) = 0;

 protected:
  virtual ~AudioLogFactory() {}
};

}  

#endif  
