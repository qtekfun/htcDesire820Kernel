// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_MEDIA_OBSERVER_H_
#define CONTENT_PUBLIC_BROWSER_MEDIA_OBSERVER_H_

#include "content/public/browser/media_request_state.h"
#include "content/public/common/media_stream_request.h"

namespace content {

class MediaObserver {
 public:
  
  virtual void OnAudioCaptureDevicesChanged(
      const MediaStreamDevices& devices) = 0;

  
  virtual void OnVideoCaptureDevicesChanged(
      const MediaStreamDevices& devices) = 0;

  
  virtual void OnMediaRequestStateChanged(
      int render_process_id,
      int render_view_id,
      int page_request_id,
      const MediaStreamDevice& device,
      MediaRequestState state) = 0;

  
  
  
  
  
  virtual void OnAudioStreamPlayingChanged(
      int render_process_id,
      int render_view_id,
      int stream_id,
      bool is_playing,
      float power_dbfs,
      bool clipped) = 0;

  
  virtual void OnCreatingAudioStream(int render_process_id,
                                     int render_view_id) = 0;

 protected:
  virtual ~MediaObserver() {}
};

}  

#endif  
