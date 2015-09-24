// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPEECH_TTS_PLATFORM_H_
#define CHROME_BROWSER_SPEECH_TTS_PLATFORM_H_

#include <string>

#include "chrome/browser/speech/tts_controller.h"

class TtsPlatformImpl {
 public:
  static TtsPlatformImpl* GetInstance();

  
  virtual bool PlatformImplAvailable() = 0;

  
  
  
  
  
  virtual bool LoadBuiltInTtsExtension(Profile* profile);

  
  
  
  
  
  
  
  virtual bool Speak(
      int utterance_id,
      const std::string& utterance,
      const std::string& lang,
      const VoiceData& voice,
      const UtteranceContinuousParameters& params) = 0;

  
  virtual bool StopSpeaking() = 0;

  
  virtual bool IsSpeaking() = 0;

  
  
  virtual void GetVoices(std::vector<VoiceData>* out_voices) = 0;

  
  
  virtual void Pause() = 0;

  
  virtual void Resume() = 0;

  virtual std::string error();
  virtual void clear_error();
  virtual void set_error(const std::string& error);

 protected:
  TtsPlatformImpl() {}

  
  
  virtual ~TtsPlatformImpl() {}

  std::string error_;

  DISALLOW_COPY_AND_ASSIGN(TtsPlatformImpl);
};

#endif  
