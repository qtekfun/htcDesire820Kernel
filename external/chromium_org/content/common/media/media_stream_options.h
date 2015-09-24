// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_MEDIA_MEDIA_STREAM_OPTIONS_H_
#define CONTENT_COMMON_MEDIA_MEDIA_STREAM_OPTIONS_H_

#include <string>
#include <vector>

#include "content/common/content_export.h"
#include "content/public/common/media_stream_request.h"

namespace content {

CONTENT_EXPORT extern const char kMediaStreamSource[];
CONTENT_EXPORT extern const char kMediaStreamSourceId[];
CONTENT_EXPORT extern const char kMediaStreamSourceInfoId[];
CONTENT_EXPORT extern const char kMediaStreamSourceTab[];
CONTENT_EXPORT extern const char kMediaStreamSourceScreen[];
CONTENT_EXPORT extern const char kMediaStreamSourceDesktop[];
CONTENT_EXPORT extern const char kMediaStreamSourceSystem[];

CONTENT_EXPORT extern const char kMediaStreamRenderToAssociatedSink[];

class CONTENT_EXPORT StreamOptions {
 public:
  StreamOptions();
  StreamOptions(bool request_audio, bool request_video);
  ~StreamOptions();

  struct CONTENT_EXPORT Constraint {
    Constraint();
    Constraint(const std::string& name,
               const std::string& value);

    std::string name;
    std::string value;
  };
  typedef std::vector<Constraint> Constraints;

  bool audio_requested;
  Constraints mandatory_audio;
  Constraints optional_audio;

  bool video_requested;
  Constraints mandatory_video;
  Constraints optional_video;

  
  
  
  
  
  
  bool GetFirstAudioConstraintByName(const std::string& name,
                                     std::string* value,
                                     bool* is_mandatory) const;

  
  
  
  
  
  
  bool GetFirstVideoConstraintByName(const std::string& name,
                                     std::string* value,
                                     bool* is_mandatory) const;

  
  
  static void GetConstraintsByName(
      const StreamOptions::Constraints& constraints,
      const std::string& name,
      std::vector<std::string>* values);
};

struct CONTENT_EXPORT StreamDeviceInfo {
  static const int kNoId;

  StreamDeviceInfo();
  StreamDeviceInfo(MediaStreamType service_param,
                   const std::string& name_param,
                   const std::string& device_param);
  StreamDeviceInfo(MediaStreamType service_param,
                   const std::string& name_param,
                   const std::string& device_param,
                   int sample_rate,
                   int channel_layout,
                   int frames_per_buffer);
  static bool IsEqual(const StreamDeviceInfo& first,
                      const StreamDeviceInfo& second);

  MediaStreamDevice device;

  
  int session_id;
};

typedef std::vector<StreamDeviceInfo> StreamDeviceInfoArray;

}  

#endif  
