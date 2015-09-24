// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SPEECH_ENDPOINTER_ENDPOINTER_H_
#define CONTENT_BROWSER_SPEECH_ENDPOINTER_ENDPOINTER_H_

#include "base/basictypes.h"
#include "content/browser/speech/endpointer/energy_endpointer.h"
#include "content/common/content_export.h"

class EpStatus;

namespace content {

class AudioChunk;

class CONTENT_EXPORT Endpointer {
 public:
  explicit Endpointer(int sample_rate);

  
  void StartSession();

  
  void EndSession();

  
  
  void SetEnvironmentEstimationMode();

  
  
  void SetUserInputMode();

  
  
  EpStatus ProcessAudio(const AudioChunk& raw_audio, float* rms_out);

  
  EpStatus Status(int64 *time_us);

  
  
  bool DidStartReceivingSpeech() const {
    return speech_previously_detected_;
  }

  bool IsEstimatingEnvironment() const {
    return energy_endpointer_.estimating_environment();
  }

  void set_speech_input_complete_silence_length(int64 time_us) {
    speech_input_complete_silence_length_us_ = time_us;
  }

  void set_long_speech_input_complete_silence_length(int64 time_us) {
    long_speech_input_complete_silence_length_us_ = time_us;
  }

  void set_speech_input_possibly_complete_silence_length(int64 time_us) {
    speech_input_possibly_complete_silence_length_us_ = time_us;
  }

  void set_long_speech_length(int64 time_us) {
    long_speech_length_us_ = time_us;
  }

  bool speech_input_complete() const {
    return speech_input_complete_;
  }

  
  float NoiseLevelDb() const { return energy_endpointer_.GetNoiseLevelDb(); }

 private:
  
  
  void Reset();

  
  int64 speech_input_minimum_length_us_;

  
  
  
  int64 speech_input_possibly_complete_silence_length_us_;

  
  
  
  int64 speech_input_complete_silence_length_us_;

  
  
  
  
  int64 long_speech_input_complete_silence_length_us_;

  
  
  
  
  int64 long_speech_length_us_;

  
  int64 speech_start_time_us_;

  
  int64 speech_end_time_us_;

  int64 audio_frame_time_us_;
  EpStatus old_ep_status_;
  bool waiting_for_speech_possibly_complete_timeout_;
  bool waiting_for_speech_complete_timeout_;
  bool speech_previously_detected_;
  bool speech_input_complete_;
  EnergyEndpointer energy_endpointer_;
  int sample_rate_;
  int32 frame_size_;
};

}  

#endif  
