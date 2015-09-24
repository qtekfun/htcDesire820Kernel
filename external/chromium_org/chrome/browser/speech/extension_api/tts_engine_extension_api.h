// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPEECH_EXTENSION_API_TTS_ENGINE_EXTENSION_API_H_
#define CHROME_BROWSER_SPEECH_EXTENSION_API_TTS_ENGINE_EXTENSION_API_H_

#include <vector>

#include "base/memory/singleton.h"
#include "chrome/browser/speech/tts_controller.h"
#include "extensions/browser/extension_function.h"

class Utterance;

namespace base {
class ListValue;
}

namespace extensions {
class Extension;
}

namespace tts_engine_events {
extern const char kOnSpeak[];
extern const char kOnStop[];
extern const char kOnPause[];
extern const char kOnResume[];
}

void GetExtensionVoices(Profile* profile, std::vector<VoiceData>* out_voices);

bool GetMatchingExtensionVoice(Utterance* utterance,
                               const extensions::Extension** matching_extension,
                               size_t* voice_index);

void ExtensionTtsEngineSpeak(Utterance* utterance,
                             const VoiceData& voice);

void ExtensionTtsEngineStop(Utterance* utterance);

void ExtensionTtsEnginePause(Utterance* utterance);

void ExtensionTtsEngineResume(Utterance* utterance);

class ExtensionTtsEngineSendTtsEventFunction : public SyncExtensionFunction {
 private:
  virtual ~ExtensionTtsEngineSendTtsEventFunction() {}
  virtual bool RunImpl() OVERRIDE;
  DECLARE_EXTENSION_FUNCTION("ttsEngine.sendTtsEvent", TTSENGINE_SENDTTSEVENT)
};

#endif  
