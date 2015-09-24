// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_MEDIA_STREAM_REQUEST_H_
#define CONTENT_PUBLIC_COMMON_MEDIA_STREAM_REQUEST_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "url/gurl.h"

namespace content {

enum MediaStreamType {
  MEDIA_NO_SERVICE = 0,

  
  MEDIA_DEVICE_AUDIO_CAPTURE,
  MEDIA_DEVICE_VIDEO_CAPTURE,

  
  MEDIA_TAB_AUDIO_CAPTURE,
  MEDIA_TAB_VIDEO_CAPTURE,

  
  MEDIA_DESKTOP_VIDEO_CAPTURE,

  
  
  
  MEDIA_LOOPBACK_AUDIO_CAPTURE,

  NUM_MEDIA_TYPES
};

enum MediaStreamRequestType {
  MEDIA_DEVICE_ACCESS = 0,
  MEDIA_GENERATE_STREAM,
  MEDIA_ENUMERATE_DEVICES,
  MEDIA_OPEN_DEVICE
};

enum VideoFacingMode {
  MEDIA_VIDEO_FACING_NONE = 0,
  MEDIA_VIDEO_FACING_USER,
  MEDIA_VIDEO_FACING_ENVIRONMENT,
  MEDIA_VIDEO_FACING_LEFT,
  MEDIA_VIDEO_FACING_RIGHT,

  NUM_MEDIA_VIDEO_FACING_MODE
};

CONTENT_EXPORT bool IsAudioMediaType(MediaStreamType type);
CONTENT_EXPORT bool IsVideoMediaType(MediaStreamType type);

struct CONTENT_EXPORT MediaStreamDevice {
  MediaStreamDevice();

  MediaStreamDevice(
      MediaStreamType type,
      const std::string& id,
      const std::string& name);

  MediaStreamDevice(
      MediaStreamType type,
      const std::string& id,
      const std::string& name,
      int sample_rate,
      int channel_layout,
      int frames_per_buffer);

  ~MediaStreamDevice();

  bool IsEqual(const MediaStreamDevice& second) const;

  
  MediaStreamType type;

  
  std::string id;

  
  VideoFacingMode video_facing;

  
  
  std::string matched_output_device_id;

  
  std::string name;

  
  
  struct AudioDeviceParameters {
    AudioDeviceParameters()
        : sample_rate(), channel_layout(), frames_per_buffer(), effects() {
    }

    AudioDeviceParameters(int sample_rate, int channel_layout,
        int frames_per_buffer)
        : sample_rate(sample_rate),
          channel_layout(channel_layout),
          frames_per_buffer(frames_per_buffer),
          effects() {
    }

    
    int sample_rate;

    
    
    
    int channel_layout;

    
    
    
    
    int frames_per_buffer;

    
    int effects;
  };

  
  

  
  AudioDeviceParameters input;

  
  
  
  
  AudioDeviceParameters matched_output;
};

typedef std::vector<MediaStreamDevice> MediaStreamDevices;

typedef std::map<MediaStreamType, MediaStreamDevices> MediaStreamDeviceMap;

struct CONTENT_EXPORT MediaStreamRequest {
  MediaStreamRequest(
      int render_process_id,
      int render_view_id,
      int page_request_id,
      const GURL& security_origin,
      MediaStreamRequestType request_type,
      const std::string& requested_audio_device_id,
      const std::string& requested_video_device_id,
      MediaStreamType audio_type,
      MediaStreamType video_type);

  ~MediaStreamRequest();

  
  
  
  int render_process_id;

  
  
  
  int render_view_id;

  
  
  int page_request_id;

  
  std::string tab_capture_device_id;

  
  GURL security_origin;

  
  
  
  
  MediaStreamRequestType request_type;

  
  std::string requested_audio_device_id;
  std::string requested_video_device_id;

  
  MediaStreamType audio_type;

  
  MediaStreamType video_type;
};

class MediaStreamUI {
 public:
  virtual ~MediaStreamUI() {}

  
  
  virtual void OnStarted(const base::Closure& stop) = 0;
};

typedef base::Callback<void(
    const MediaStreamDevices& devices,
    scoped_ptr<MediaStreamUI> ui)> MediaResponseCallback;

}  

#endif  
