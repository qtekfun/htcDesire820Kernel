// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_MEDIA_STREAM_AUDIO_RENDERER_H_
#define CONTENT_RENDERER_MEDIA_MEDIA_STREAM_AUDIO_RENDERER_H_

#include "base/memory/ref_counted.h"
#include "base/time/time.h"

namespace content {

class MediaStreamAudioRenderer
    : public base::RefCountedThreadSafe<MediaStreamAudioRenderer> {
 public:
  
  virtual void Start() = 0;

  
  virtual void Stop() = 0;

  
  virtual void Play() = 0;

  
  
  virtual void Pause() = 0;

  
  virtual void SetVolume(float volume) = 0;

  
  
  virtual base::TimeDelta GetCurrentRenderTime() const = 0;

  
  
  virtual bool IsLocalRenderer() const = 0;

 protected:
  friend class base::RefCountedThreadSafe<MediaStreamAudioRenderer>;

  MediaStreamAudioRenderer();
  virtual ~MediaStreamAudioRenderer();

 private:
  DISALLOW_COPY_AND_ASSIGN(MediaStreamAudioRenderer);
};

}  

#endif  
