// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_AUDIO_CAPTURER_LINUX_H_
#define REMOTING_HOST_AUDIO_CAPTURER_LINUX_H_

#include "base/memory/ref_counted.h"
#include "remoting/host/audio_capturer.h"
#include "remoting/host/audio_silence_detector.h"
#include "remoting/host/linux/audio_pipe_reader.h"

namespace base {
class FilePath;
}

namespace remoting {

class AudioCapturerLinux : public AudioCapturer,
                           public AudioPipeReader::StreamObserver {
 public:
  
  
  
  static void InitializePipeReader(
      scoped_refptr<base::SingleThreadTaskRunner> task_runner,
      const base::FilePath& pipe_name);

  explicit AudioCapturerLinux(
      scoped_refptr<AudioPipeReader> pipe_reader);
  virtual ~AudioCapturerLinux();

  
  virtual bool Start(const PacketCapturedCallback& callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual bool IsStarted() OVERRIDE;

  
  virtual void OnDataRead(scoped_refptr<base::RefCountedString> data) OVERRIDE;

 private:
  scoped_refptr<AudioPipeReader> pipe_reader_;
  PacketCapturedCallback callback_;

  AudioSilenceDetector silence_detector_;

  DISALLOW_COPY_AND_ASSIGN(AudioCapturerLinux);
};

}  

#endif  
