// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_MEDIA_LOG_EVENT_H_
#define MEDIA_BASE_MEDIA_LOG_EVENT_H_

#include "base/time/time.h"
#include "base/values.h"

namespace media {

struct MediaLogEvent {
  MediaLogEvent() {}

  MediaLogEvent(const MediaLogEvent& event) {
    *this = event;
  }

  MediaLogEvent& operator=(const MediaLogEvent& event) {
    id = event.id;
    type = event.type;
    scoped_ptr<base::DictionaryValue> event_copy(event.params.DeepCopy());
    params.Swap(event_copy.get());
    time = event.time;
    return *this;
  }

  enum Type {
    
    
    WEBMEDIAPLAYER_CREATED,
    WEBMEDIAPLAYER_DESTROYED,

    
    
    PIPELINE_CREATED,
    PIPELINE_DESTROYED,

    
    
    LOAD,

    
    
    SEEK,

    
    
    PLAY,
    PAUSE,

    
    
    PIPELINE_STATE_CHANGED,

    
    
    PIPELINE_ERROR,

    
    
    
    VIDEO_SIZE_SET,

    
    
    
    DURATION_SET,
    TOTAL_BYTES_SET,
    NETWORK_ACTIVITY_SET,

    
    AUDIO_ENDED,
    VIDEO_ENDED,
    TEXT_ENDED,

    
    
    AUDIO_RENDERER_DISABLED,

    
    
    
    
    BUFFERED_EXTENTS_CHANGED,

    
    MEDIA_SOURCE_ERROR,
    

    
    PROPERTY_CHANGE,
  };

  int32 id;
  Type type;
  base::DictionaryValue params;
  base::TimeTicks time;
};

}  

#endif  
