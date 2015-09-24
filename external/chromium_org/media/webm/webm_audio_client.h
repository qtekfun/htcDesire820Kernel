// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_WEBM_WEBM_AUDIO_CLIENT_H_
#define MEDIA_WEBM_WEBM_AUDIO_CLIENT_H_

#include <string>
#include <vector>

#include "media/base/media_log.h"
#include "media/webm/webm_parser.h"

namespace media {
class AudioDecoderConfig;

class WebMAudioClient : public WebMParserClient {
 public:
  explicit WebMAudioClient(const LogCB& log_cb);
  virtual ~WebMAudioClient();

  
  void Reset();

  
  
  
  
  
  
  bool InitializeConfig(const std::string& codec_id,
                        const std::vector<uint8>& codec_private,
                        const int64 seek_preroll,
                        const int64 codec_delay,
                        bool is_encrypted,
                        AudioDecoderConfig* config);

 private:
  
  virtual bool OnUInt(int id, int64 val) OVERRIDE;
  virtual bool OnFloat(int id, double val) OVERRIDE;

  LogCB log_cb_;
  int channels_;
  double samples_per_second_;
  double output_samples_per_second_;

  DISALLOW_COPY_AND_ASSIGN(WebMAudioClient);
};

}  

#endif  
