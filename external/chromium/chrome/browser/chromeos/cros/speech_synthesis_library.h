// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_SPEECH_SYNTHESIS_LIBRARY_H_
#define CHROME_BROWSER_CHROMEOS_CROS_SPEECH_SYNTHESIS_LIBRARY_H_
#pragma once

#include "base/memory/singleton.h"

namespace chromeos {

class SpeechSynthesisLibrary {
 public:
  typedef void(*InitStatusCallback)(bool success);

  virtual ~SpeechSynthesisLibrary() {}

  
  virtual bool Speak(const char* text) = 0;

  
  
  virtual bool SetSpeakProperties(const char* props) = 0;

  
  virtual bool StopSpeaking() = 0;

  
  virtual bool IsSpeaking() = 0;

  
  
  virtual void InitTts(InitStatusCallback) = 0;

  
  
  static SpeechSynthesisLibrary* GetImpl(bool stub);

  
  static const char kSpeechPropertyLocale[];
  static const char kSpeechPropertyGender[];
  static const char kSpeechPropertyRate[];
  static const char kSpeechPropertyPitch[];
  static const char kSpeechPropertyVolume[];
  static const char kSpeechPropertyEquals[];
  static const char kSpeechPropertyDelimiter[];
};

}  

#endif  
