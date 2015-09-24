// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_AUDIO_SOURCE_DIVERTER_H_
#define MEDIA_AUDIO_AUDIO_SOURCE_DIVERTER_H_

#include "media/base/media_export.h"


namespace media {

class AudioOutputStream;
class AudioParameters;

class MEDIA_EXPORT AudioSourceDiverter {
public:
  
  virtual const AudioParameters& GetAudioParameters() = 0;

  
  
  
  virtual void StartDiverting(AudioOutputStream* to_stream) = 0;

  
  
  virtual void StopDiverting() = 0;

protected:
  virtual ~AudioSourceDiverter() {}
};

}  

#endif  
