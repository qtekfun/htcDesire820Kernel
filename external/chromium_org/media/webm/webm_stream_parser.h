// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_WEBM_WEBM_STREAM_PARSER_H_
#define MEDIA_WEBM_WEBM_STREAM_PARSER_H_

#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "media/base/audio_decoder_config.h"
#include "media/base/buffers.h"
#include "media/base/byte_queue.h"
#include "media/base/stream_parser.h"
#include "media/base/video_decoder_config.h"

namespace media {

class WebMClusterParser;

class WebMStreamParser : public StreamParser {
 public:
  WebMStreamParser();
  virtual ~WebMStreamParser();

  
  virtual void Init(const InitCB& init_cb, const NewConfigCB& config_cb,
                    const NewBuffersCB& new_buffers_cb,
                    const NewTextBuffersCB& text_cb,
                    const NeedKeyCB& need_key_cb,
                    const NewMediaSegmentCB& new_segment_cb,
                    const base::Closure& end_of_segment_cb,
                    const LogCB& log_cb) OVERRIDE;
  virtual void Flush() OVERRIDE;
  virtual bool Parse(const uint8* buf, int size) OVERRIDE;

 private:
  enum State {
    kWaitingForInit,
    kParsingHeaders,
    kParsingClusters,
    kError
  };

  void ChangeState(State new_state);

  
  
  
  
  
  
  
  
  int ParseInfoAndTracks(const uint8* data, int size);

  
  
  
  
  
  
  
  int ParseCluster(const uint8* data, int size);

  
  void FireNeedKey(const std::string& key_id);

  State state_;
  InitCB init_cb_;
  NewConfigCB config_cb_;
  NewBuffersCB new_buffers_cb_;
  NewTextBuffersCB text_cb_;
  NeedKeyCB need_key_cb_;

  NewMediaSegmentCB new_segment_cb_;
  base::Closure end_of_segment_cb_;
  LogCB log_cb_;

  
  
  bool waiting_for_buffers_;

  scoped_ptr<WebMClusterParser> cluster_parser_;
  ByteQueue byte_queue_;

  DISALLOW_COPY_AND_ASSIGN(WebMStreamParser);
};

}  

#endif  
