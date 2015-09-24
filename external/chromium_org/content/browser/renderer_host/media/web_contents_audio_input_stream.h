// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_WEB_CONTENTS_AUDIO_INPUT_STREAM_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_WEB_CONTENTS_AUDIO_INPUT_STREAM_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "media/audio/audio_io.h"

namespace base {
class MessageLoopProxy;
}

namespace media {
class AudioParameters;
class VirtualAudioInputStream;
}

namespace content {

class AudioMirroringManager;
class WebContentsTracker;

class CONTENT_EXPORT WebContentsAudioInputStream
    : NON_EXPORTED_BASE(public media::AudioInputStream) {
 public:
  
  virtual bool Open() OVERRIDE;
  virtual void Start(AudioInputCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual double GetMaxVolume() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual double GetVolume() OVERRIDE;
  virtual void SetAutomaticGainControl(bool enabled) OVERRIDE;
  virtual bool GetAutomaticGainControl() OVERRIDE;

  
  
  
  
  
  
  
  
  static WebContentsAudioInputStream* Create(
      const std::string& device_id,
      const media::AudioParameters& params,
      const scoped_refptr<base::MessageLoopProxy>& worker_loop,
      AudioMirroringManager* audio_mirroring_manager);

 private:
  friend class WebContentsAudioInputStreamTest;

  
  
  
  
  
  class Impl;

  WebContentsAudioInputStream(
      int render_process_id, int render_view_id,
      AudioMirroringManager* mirroring_manager,
      const scoped_refptr<WebContentsTracker>& tracker,
      media::VirtualAudioInputStream* mixer_stream);

  virtual ~WebContentsAudioInputStream();

  scoped_refptr<Impl> impl_;

  DISALLOW_COPY_AND_ASSIGN(WebContentsAudioInputStream);
};

}  

#endif  
