// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_SPEECH_RECOGNITION_EVENT_LISTENER_H_
#define CONTENT_PUBLIC_BROWSER_SPEECH_RECOGNITION_EVENT_LISTENER_H_

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "content/public/common/speech_recognition_result.h"

namespace content {

struct SpeechRecognitionError;

class CONTENT_EXPORT SpeechRecognitionEventListener {
 public:
  
  
  virtual void OnRecognitionStart(int session_id) = 0;

  
  virtual void OnAudioStart(int session_id) = 0;

  
  
  
  
  virtual void OnEnvironmentEstimationComplete(int session_id) = 0;

  
  virtual void OnSoundStart(int session_id) = 0;

  
  virtual void OnSoundEnd(int session_id) = 0;

  
  
  virtual void OnAudioEnd(int session_id) = 0;

  
  virtual void OnRecognitionResults(int session_id,
      const SpeechRecognitionResults& results) = 0;

  
  
  
  virtual void OnRecognitionError(int session_id,
                                  const SpeechRecognitionError& error) = 0;

  
  
  
  
  
  virtual void OnAudioLevelsChange(int session_id,
                                   float volume, float noise_volume) = 0;

  
  
  virtual void OnRecognitionEnd(int session_id) = 0;

 protected:
  virtual ~SpeechRecognitionEventListener() {}
};

}  

#endif  
