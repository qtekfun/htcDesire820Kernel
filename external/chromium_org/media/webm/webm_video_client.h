// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_WEBM_WEBM_VIDEO_CLIENT_H_
#define MEDIA_WEBM_WEBM_VIDEO_CLIENT_H_

#include <string>
#include <vector>

#include "media/base/media_log.h"
#include "media/webm/webm_parser.h"

namespace media {
class VideoDecoderConfig;

class WebMVideoClient : public WebMParserClient {
 public:
  explicit WebMVideoClient(const LogCB& log_cb);
  virtual ~WebMVideoClient();

  
  void Reset();

  
  
  
  
  
  
  
  bool InitializeConfig(const std::string& codec_id,
                        const std::vector<uint8>& codec_private,
                        bool is_encrypted,
                        VideoDecoderConfig* config);

 private:
  
  virtual bool OnUInt(int id, int64 val) OVERRIDE;
  virtual bool OnBinary(int id, const uint8* data, int size) OVERRIDE;
  virtual bool OnFloat(int id, double val) OVERRIDE;

  LogCB log_cb_;
  int64 pixel_width_;
  int64 pixel_height_;
  int64 crop_bottom_;
  int64 crop_top_;
  int64 crop_left_;
  int64 crop_right_;
  int64 display_width_;
  int64 display_height_;
  int64 display_unit_;
  int64 alpha_mode_;

  DISALLOW_COPY_AND_ASSIGN(WebMVideoClient);
};

}  

#endif  
