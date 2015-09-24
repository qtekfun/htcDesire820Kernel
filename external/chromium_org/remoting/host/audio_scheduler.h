// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_AUDIO_SCHEDULER_H_
#define REMOTING_HOST_AUDIO_SCHEDULER_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace remoting {

namespace protocol {
class AudioStub;
}  

class AudioCapturer;
class AudioEncoder;
class AudioPacket;

class AudioScheduler : public base::RefCountedThreadSafe<AudioScheduler> {
 public:
  
  
  
  
  AudioScheduler(
      scoped_refptr<base::SingleThreadTaskRunner> audio_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> network_task_runner,
      scoped_ptr<AudioCapturer> audio_capturer,
      scoped_ptr<AudioEncoder> audio_encoder,
      protocol::AudioStub* audio_stub);

  
  void Start();

  
  void Stop();

  
  
  
  void Pause(bool pause);

 private:
  friend class base::RefCountedThreadSafe<AudioScheduler>;
  virtual ~AudioScheduler();

  
  void StartOnAudioThread();

  
  void StopOnAudioThread();

  
  void EncodeAudioPacket(scoped_ptr<AudioPacket> packet);

  
  void SendAudioPacket(scoped_ptr<AudioPacket> packet);

  scoped_refptr<base::SingleThreadTaskRunner> audio_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> network_task_runner_;

  scoped_ptr<AudioCapturer> audio_capturer_;

  scoped_ptr<AudioEncoder> audio_encoder_;

  protocol::AudioStub* audio_stub_;

  bool network_stopped_;

  bool enabled_;

  DISALLOW_COPY_AND_ASSIGN(AudioScheduler);
};

}  

#endif  
