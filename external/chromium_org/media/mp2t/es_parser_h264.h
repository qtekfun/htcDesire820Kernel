// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MP2T_ES_PARSER_H264_H_
#define MEDIA_MP2T_ES_PARSER_H264_H_

#include <list>
#include <utility>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/time/time.h"
#include "media/base/byte_queue.h"
#include "media/base/video_decoder_config.h"
#include "media/mp2t/es_parser.h"

namespace media {
class BitReader;
class StreamParserBuffer;
}

namespace media {
namespace mp2t {

class EsParserH264 : public EsParser {
 public:
  typedef base::Callback<void(const VideoDecoderConfig&)> NewVideoConfigCB;

  EsParserH264(const NewVideoConfigCB& new_video_config_cb,
               const EmitBufferCB& emit_buffer_cb);
  virtual ~EsParserH264();

  
  virtual bool Parse(const uint8* buf, int size,
                     base::TimeDelta pts,
                     base::TimeDelta dts) OVERRIDE;
  virtual void Flush() OVERRIDE;
  virtual void Reset() OVERRIDE;

 private:
  struct TimingDesc {
    base::TimeDelta dts;
    base::TimeDelta pts;
  };

  
  
  bool ParseInternal();

  
  void EmitFrameIfNeeded(int next_aud_pos);

  
  
  void StartFrame(int aud_pos);

  
  void DiscardEs(int nbytes);

  
  
  bool NalParser(const uint8* buf, int size);
  bool ProcessSPS(const uint8* buf, int size);

  
  NewVideoConfigCB new_video_config_cb_;
  EmitBufferCB emit_buffer_cb_;

  
  ByteQueue es_byte_queue_;
  std::list<std::pair<int, TimingDesc> > timing_desc_list_;

  
  
  
  
  int es_pos_;
  int current_nal_pos_;
  int current_access_unit_pos_;
  bool is_key_frame_;

  
  VideoDecoderConfig last_video_decoder_config_;
};

}  
}  

#endif

