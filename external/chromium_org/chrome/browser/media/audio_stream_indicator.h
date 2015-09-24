// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_AUDIO_STREAM_INDICATOR_H_
#define CHROME_BROWSER_MEDIA_AUDIO_STREAM_INDICATOR_H_

#include <map>
#include <vector>

#include "base/memory/ref_counted.h"

namespace content {
class WebContents;
}

class AudioStreamIndicator
    : public base::RefCountedThreadSafe<AudioStreamIndicator> {
 public:
  AudioStreamIndicator();

  
  void UpdateWebContentsStatus(int render_process_id,
                               int render_view_id,
                               int stream_id,
                               bool is_playing,
                               float power_dbfs,
                               bool clipped);

  
  bool IsPlayingAudio(const content::WebContents* contents);

  
  
  
  
  
  
  void CurrentAudibleLevel(const content::WebContents* contents,
                           float* level, bool* signal_has_clipped);

 private:
  
  
  typedef std::pair<int, int> RenderViewId;
  struct StreamPowerLevel {
    int stream_id;
    float power_dbfs;
    bool clipped;
  };
  typedef std::vector<StreamPowerLevel> StreamPowerLevels;
  
  typedef std::map<RenderViewId, StreamPowerLevels> RenderViewStreamMap;

  friend class base::RefCountedThreadSafe<AudioStreamIndicator>;
  virtual ~AudioStreamIndicator();

  void UpdateWebContentsStatusOnUIThread(int render_process_id,
                                         int render_view_id,
                                         int stream_id,
                                         bool is_playing,
                                         float power_dbfs,
                                         bool clipped);

  RenderViewStreamMap audio_streams_;
};

#endif  
