// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_VIRTUAL_AUDIO_INPUT_STREAM_H_
#define MEDIA_AUDIO_VIRTUAL_AUDIO_INPUT_STREAM_H_

#include <map>
#include <set>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"
#include "media/audio/fake_audio_consumer.h"
#include "media/base/audio_converter.h"

namespace base {
class MessageLoopProxy;
}

namespace media {

class LoopbackAudioConverter;
class VirtualAudioOutputStream;

class MEDIA_EXPORT VirtualAudioInputStream : public AudioInputStream {
 public:
  
  typedef base::Callback<void(VirtualAudioInputStream* vais)>
      AfterCloseCallback;

  
  
  
  
  VirtualAudioInputStream(
      const AudioParameters& params,
      const scoped_refptr<base::MessageLoopProxy>& worker_loop,
      const AfterCloseCallback& after_close_cb);

  virtual ~VirtualAudioInputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Start(AudioInputCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual double GetMaxVolume() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual double GetVolume() OVERRIDE;
  virtual void SetAutomaticGainControl(bool enabled) OVERRIDE;
  virtual bool GetAutomaticGainControl() OVERRIDE;

  
  
  
  
  virtual void AddOutputStream(VirtualAudioOutputStream* stream,
                               const AudioParameters& output_params);

  
  virtual void RemoveOutputStream(VirtualAudioOutputStream* stream,
                                  const AudioParameters& output_params);

 private:
  friend class VirtualAudioInputStreamTest;

  typedef std::map<AudioParameters, LoopbackAudioConverter*> AudioConvertersMap;

  
  
  
  void PumpAudio(AudioBus* audio_bus);

  const scoped_refptr<base::MessageLoopProxy> worker_loop_;

  AfterCloseCallback after_close_cb_;

  AudioInputCallback* callback_;

  
  scoped_ptr<uint8[]> buffer_;
  AudioParameters params_;

  
  
  base::Lock converter_network_lock_;

  
  
  AudioConvertersMap converters_;

  
  
  AudioConverter mixer_;

  
  int num_attached_output_streams_;

  
  FakeAudioConsumer fake_consumer_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(VirtualAudioInputStream);
};

}  

#endif  
