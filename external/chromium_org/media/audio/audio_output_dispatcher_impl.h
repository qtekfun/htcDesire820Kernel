// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_AUDIO_AUDIO_OUTPUT_DISPATCHER_IMPL_H_
#define MEDIA_AUDIO_AUDIO_OUTPUT_DISPATCHER_IMPL_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/timer/timer.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_logging.h"
#include "media/audio/audio_manager.h"
#include "media/audio/audio_output_dispatcher.h"
#include "media/audio/audio_parameters.h"

namespace media {

class AudioOutputProxy;

class MEDIA_EXPORT AudioOutputDispatcherImpl : public AudioOutputDispatcher {
 public:
  
  
  AudioOutputDispatcherImpl(AudioManager* audio_manager,
                            const AudioParameters& params,
                            const std::string& output_device_id,
                            const std::string& input_device_id,
                            const base::TimeDelta& close_delay);

  
  
  virtual bool OpenStream() OVERRIDE;

  
  
  virtual bool StartStream(AudioOutputStream::AudioSourceCallback* callback,
                           AudioOutputProxy* stream_proxy) OVERRIDE;

  
  
  virtual void StopStream(AudioOutputProxy* stream_proxy) OVERRIDE;

  virtual void StreamVolumeSet(AudioOutputProxy* stream_proxy,
                               double volume) OVERRIDE;

  
  
  
  virtual void CloseStream(AudioOutputProxy* stream_proxy) OVERRIDE;

  virtual void Shutdown() OVERRIDE;

  virtual void CloseStreamsForWedgeFix() OVERRIDE;
  virtual void RestartStreamsForWedgeFix() OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<AudioOutputDispatcherImpl>;
  virtual ~AudioOutputDispatcherImpl();

  
  
  
  bool CreateAndOpenStream();

  
  void CloseAllIdleStreams();
  
  void CloseIdleStreams(size_t keep_alive);

  size_t idle_proxies_;
  std::vector<AudioOutputStream*> idle_streams_;

  
  
  
  base::DelayTimer<AudioOutputDispatcherImpl> close_timer_;

  typedef std::map<AudioOutputProxy*, AudioOutputStream*> AudioStreamMap;
  AudioStreamMap proxy_to_physical_map_;

  scoped_ptr<AudioLog> audio_log_;
  typedef std::map<AudioOutputStream*, int> AudioStreamIDMap;
  AudioStreamIDMap audio_stream_ids_;
  int audio_stream_id_;

  DISALLOW_COPY_AND_ASSIGN(AudioOutputDispatcherImpl);
};

}  

#endif  
