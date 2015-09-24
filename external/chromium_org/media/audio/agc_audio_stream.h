// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_AGC_AUDIO_STREAM_H_
#define MEDIA_AUDIO_AGC_AUDIO_STREAM_H_

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "base/timer/timer.h"
#include "media/audio/audio_io.h"


namespace media {

template <typename AudioInterface>
class MEDIA_EXPORT AgcAudioStream : public AudioInterface {
 public:
  
  static const int kIntervalBetweenVolumeUpdatesMs = 1000;

  AgcAudioStream()
      : agc_is_enabled_(false), max_volume_(0.0), normalized_volume_(0.0) {
    DVLOG(1) << __FUNCTION__;
  }

  virtual ~AgcAudioStream() {
    DCHECK(thread_checker_.CalledOnValidThread());
    DVLOG(1) << __FUNCTION__;
  }

 protected:
  
  
  
  
  void StartAgc() {
    DVLOG(1) << "StartAgc()";
    DCHECK(thread_checker_.CalledOnValidThread());
    if (!agc_is_enabled_ || timer_.IsRunning())
      return;

    
    
    
    QueryAndStoreNewMicrophoneVolume();

    timer_.Start(FROM_HERE,
        base::TimeDelta::FromMilliseconds(kIntervalBetweenVolumeUpdatesMs),
        this, &AgcAudioStream::QueryAndStoreNewMicrophoneVolume);
  }

  
  
  void StopAgc() {
    DVLOG(1) << "StopAgc()";
    DCHECK(thread_checker_.CalledOnValidThread());
    if (timer_.IsRunning())
      timer_.Stop();
  }

  
  
  void UpdateAgcVolume() {
    DCHECK(thread_checker_.CalledOnValidThread());

    if (!timer_.IsRunning())
      return;

    
    
    
    
    
    
    QueryAndStoreNewMicrophoneVolume();
  }

  
  
  
  void GetAgcVolume(double* normalized_volume) {
    base::AutoLock lock(lock_);
    *normalized_volume = normalized_volume_;
  }

 private:
  
  
  
  
  
  virtual void SetAutomaticGainControl(bool enabled) OVERRIDE {
    DVLOG(1) << "SetAutomaticGainControl(enabled=" << enabled << ")";
    DCHECK(thread_checker_.CalledOnValidThread());
    agc_is_enabled_ = enabled;
  }

  
  virtual bool GetAutomaticGainControl() OVERRIDE {
    DCHECK(thread_checker_.CalledOnValidThread());
    return agc_is_enabled_;
  }

  
  
  
  
  
  
  
  void QueryAndStoreNewMicrophoneVolume() {
    DCHECK(thread_checker_.CalledOnValidThread());

    
    if (max_volume_ == 0.0)
      max_volume_ = static_cast<AudioInterface*>(this)->GetMaxVolume();

    
    
    if (max_volume_ != 0.0) {
      double normalized_volume =
          static_cast<AudioInterface*>(this)->GetVolume() / max_volume_;
      base::AutoLock auto_lock(lock_);
      normalized_volume_ = normalized_volume;
    }
  }

  
  base::ThreadChecker thread_checker_;

  
  
  base::RepeatingTimer<AgcAudioStream<AudioInterface> > timer_;

  
  bool agc_is_enabled_;

  
  
  double max_volume_;

  
  
  
  double normalized_volume_;

  
  base::Lock lock_;

  DISALLOW_COPY_AND_ASSIGN(AgcAudioStream<AudioInterface>);
};

}  

#endif  
