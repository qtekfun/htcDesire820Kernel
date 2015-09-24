// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_AUDIO_CAPTURER_H_
#define REMOTING_HOST_AUDIO_CAPTURER_H_

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"

namespace remoting {

class AudioPacket;

class AudioCapturer {
 public:
  typedef base::Callback<void(scoped_ptr<AudioPacket> packet)>
      PacketCapturedCallback;

  virtual ~AudioCapturer() {}

  
  
  static bool IsSupported();
  static scoped_ptr<AudioCapturer> Create();

  
  
  
  virtual bool Start(const PacketCapturedCallback& callback) = 0;
  
  
  virtual void Stop() = 0;
  
  virtual bool IsStarted() = 0;

  static bool IsValidSampleRate(int sample_rate);
};

}  

#endif  
