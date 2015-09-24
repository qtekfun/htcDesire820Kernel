// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SPEECH_SPEECH_RECOGNITION_ENGINE_H_
#define CONTENT_BROWSER_SPEECH_SPEECH_RECOGNITION_ENGINE_H_

#include <string>

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "content/public/common/speech_recognition_grammar.h"
#include "content/public/common/speech_recognition_result.h"

namespace content {

class AudioChunk;
struct SpeechRecognitionError;

class SpeechRecognitionEngine {
 public:
  
  class Delegate {
   public:
    
    
    
    virtual void OnSpeechRecognitionEngineResults(
        const SpeechRecognitionResults& results) = 0;
    virtual void OnSpeechRecognitionEngineError(
        const SpeechRecognitionError& error) = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  struct CONTENT_EXPORT Config {
    Config();
    ~Config();

    std::string language;
    SpeechRecognitionGrammarArray grammars;
    bool filter_profanities;
    bool continuous;
    bool interim_results;
    uint32 max_hypotheses;
    std::string hardware_info;
    std::string origin_url;
    int audio_sample_rate;
    int audio_num_bits_per_sample;
  };

  virtual ~SpeechRecognitionEngine() {}

  
  
  virtual void SetConfig(const Config& config) = 0;

  
  virtual void StartRecognition() = 0;

  
  
  
  
  virtual void EndRecognition() = 0;

  
  
  virtual void TakeAudioChunk(const AudioChunk& data) = 0;

  
  
  
  virtual void AudioChunksEnded() = 0;

  
  virtual bool IsRecognitionPending() const = 0;

  
  virtual int GetDesiredAudioChunkDurationMs() const = 0;

  
  void set_delegate(Delegate* delegate) { delegate_ = delegate; }

 protected:
  Delegate* delegate() const { return delegate_; }

 private:
  Delegate* delegate_;
};

typedef SpeechRecognitionEngine::Delegate SpeechRecognitionEngineDelegate;
typedef SpeechRecognitionEngine::Config SpeechRecognitionEngineConfig;

}  

#endif  
