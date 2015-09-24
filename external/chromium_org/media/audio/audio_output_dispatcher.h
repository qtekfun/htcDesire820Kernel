// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_AUDIO_AUDIO_OUTPUT_DISPATCHER_H_
#define MEDIA_AUDIO_AUDIO_OUTPUT_DISPATCHER_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/timer/timer.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_manager.h"
#include "media/audio/audio_parameters.h"

namespace base {
class MessageLoopProxy;
}

namespace media {

class AudioOutputProxy;

class MEDIA_EXPORT AudioOutputDispatcher
    : public base::RefCountedThreadSafe<AudioOutputDispatcher> {
 public:
  AudioOutputDispatcher(AudioManager* audio_manager,
                        const AudioParameters& params,
                        const std::string& output_device_id,
                        const std::string& input_device_id);

  
  
  virtual bool OpenStream() = 0;

  
  
  
  
  virtual bool StartStream(AudioOutputStream::AudioSourceCallback* callback,
                           AudioOutputProxy* stream_proxy) = 0;

  
  
  virtual void StopStream(AudioOutputProxy* stream_proxy) = 0;

  
  virtual void StreamVolumeSet(AudioOutputProxy* stream_proxy,
                               double volume) = 0;

  
  virtual void CloseStream(AudioOutputProxy* stream_proxy) = 0;

  
  virtual void Shutdown() = 0;

  
  
  
  
  virtual void CloseStreamsForWedgeFix() = 0;
  virtual void RestartStreamsForWedgeFix() = 0;

  
  const std::string& input_device_id() const { return input_device_id_; }

 protected:
  friend class base::RefCountedThreadSafe<AudioOutputDispatcher>;
  virtual ~AudioOutputDispatcher();

  
  
  AudioManager* audio_manager_;
  const scoped_refptr<base::MessageLoopProxy> message_loop_;
  const AudioParameters params_;
  std::string output_device_id_;
  const std::string input_device_id_;

 private:
  DISALLOW_COPY_AND_ASSIGN(AudioOutputDispatcher);
};

}  

#endif  
