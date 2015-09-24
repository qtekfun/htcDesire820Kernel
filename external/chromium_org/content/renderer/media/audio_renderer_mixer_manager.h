// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_AUDIO_RENDERER_MIXER_MANAGER_H_
#define CONTENT_RENDERER_MEDIA_AUDIO_RENDERER_MIXER_MANAGER_H_

#include <map>
#include <utility>

#include "base/synchronization/lock.h"
#include "content/common/content_export.h"
#include "media/audio/audio_parameters.h"

namespace media {
class AudioHardwareConfig;
class AudioRendererMixer;
class AudioRendererMixerInput;
class AudioRendererSink;
}

namespace content {

class CONTENT_EXPORT AudioRendererMixerManager {
 public:
  
  
  
  explicit AudioRendererMixerManager(
      media::AudioHardwareConfig* hardware_config);
  ~AudioRendererMixerManager();

  
  
  
  
  
  media::AudioRendererMixerInput* CreateInput(int source_render_view_id);

  
  
  media::AudioRendererMixer* GetMixer(int source_render_view_id,
                                      const media::AudioParameters& params);

  
  
  
  void RemoveMixer(int source_render_view_id,
                   const media::AudioParameters& params);

 private:
  friend class AudioRendererMixerManagerTest;

  
  
  typedef std::pair<int, media::AudioParameters> MixerKey;

  
  
  
  struct AudioRendererMixerReference {
    media::AudioRendererMixer* mixer;
    int ref_count;
  };
  typedef std::map<MixerKey, AudioRendererMixerReference> AudioRendererMixerMap;

  
  void SetAudioRendererSinkForTesting(media::AudioRendererSink* sink);

  
  AudioRendererMixerMap mixers_;
  base::Lock mixers_lock_;

  
  
  media::AudioHardwareConfig* const hardware_config_;

  media::AudioRendererSink* sink_for_testing_;

  DISALLOW_COPY_AND_ASSIGN(AudioRendererMixerManager);
};

}  

#endif  
