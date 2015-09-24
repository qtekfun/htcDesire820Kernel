// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_AUDIO_MIRRORING_MANAGER_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_AUDIO_MIRRORING_MANAGER_H_

#include <map>
#include <utility>

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "media/audio/audio_source_diverter.h"

namespace media {
class AudioOutputStream;
}

namespace content {

class CONTENT_EXPORT AudioMirroringManager {
 public:
  
  typedef media::AudioSourceDiverter Diverter;

  
  
  class MirroringDestination {
   public:
    
    
    
    
    virtual media::AudioOutputStream* AddInput(
        const media::AudioParameters& params) = 0;

   protected:
    virtual ~MirroringDestination() {}
  };

  AudioMirroringManager();

  virtual ~AudioMirroringManager();

  
  
  
  
  
  
  
  virtual void AddDiverter(int render_process_id, int render_view_id,
                           Diverter* diverter);
  virtual void RemoveDiverter(int render_process_id, int render_view_id,
                              Diverter* diverter);

  
  
  
  
  virtual void StartMirroring(int render_process_id, int render_view_id,
                              MirroringDestination* destination);
  virtual void StopMirroring(int render_process_id, int render_view_id,
                             MirroringDestination* destination);

 private:
  
  
  typedef std::pair<int, int> Target;

  
  typedef std::multimap<Target, Diverter*> DiverterMap;
  typedef std::map<Target, MirroringDestination*> SessionMap;

  
  DiverterMap diverters_;

  
  SessionMap sessions_;

  DISALLOW_COPY_AND_ASSIGN(AudioMirroringManager);
};

}  

#endif  
