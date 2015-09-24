// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// As written, the basic idea is that one specifies intervals that

#ifndef CONTENT_BROWSER_SPEECH_ENDPOINTER_ENERGY_ENDPOINTER_H_
#define CONTENT_BROWSER_SPEECH_ENDPOINTER_ENERGY_ENDPOINTER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/browser/speech/endpointer/energy_endpointer_params.h"
#include "content/common/content_export.h"

namespace content {

enum EpStatus {
  EP_PRE_SPEECH = 10,
  EP_POSSIBLE_ONSET,
  EP_SPEECH_PRESENT,
  EP_POSSIBLE_OFFSET,
  EP_POST_SPEECH,
};

class CONTENT_EXPORT EnergyEndpointer {
 public:
  
  
  EnergyEndpointer();
  virtual ~EnergyEndpointer();

  void Init(const EnergyEndpointerParams& params);

  
  void StartSession();

  
  void EndSession();

  
  
  void SetEnvironmentEstimationMode();

  
  
  void SetUserInputMode();

  
  
  void ProcessAudioFrame(int64 time_us,
                         const int16* samples, int num_samples,
                         float* rms_out);

  
  
  EpStatus Status(int64* status_time_us) const;

  bool estimating_environment() const {
    return estimating_environment_;
  }

  
  float GetNoiseLevelDb() const;

 private:
  class HistoryRing;

  
  
  
  void Restart(bool reset_threshold);

  
  void UpdateLevels(float rms);

  
  
  int TimeToFrame(float time) const;

  EpStatus status_;  
  float offset_confirm_dur_sec_;  
  int64 endpointer_time_us_;  
  int64 fast_update_frames_; 
  int64 frame_counter_;  
  float max_window_dur_;  
  float sample_rate_;  

  
  scoped_ptr<HistoryRing> history_;

  
  EnergyEndpointerParams params_;

  
  float decision_threshold_;

  
  
  bool estimating_environment_;

  
  float noise_level_;

  
  float rms_adapt_;

  
  int start_lag_;

  
  int end_lag_;

  
  
  int64 user_input_start_time_us_;

  DISALLOW_COPY_AND_ASSIGN(EnergyEndpointer);
};

}  

#endif  
