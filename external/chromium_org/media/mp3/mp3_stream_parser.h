// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MP3_MP3_STREAM_PARSER_H_
#define MEDIA_MP3_MP3_STREAM_PARSER_H_

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "media/base/audio_decoder_config.h"
#include "media/base/audio_timestamp_helper.h"
#include "media/base/byte_queue.h"
#include "media/base/media_export.h"
#include "media/base/stream_parser.h"

namespace media {

class BitReader;

class MEDIA_EXPORT MP3StreamParser : public StreamParser {
 public:
  MP3StreamParser();
  virtual ~MP3StreamParser();

  
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
    UNINITIALIZED,
    INITIALIZED,
    PARSE_ERROR
  };

  State state_;

  InitCB init_cb_;
  NewConfigCB config_cb_;
  NewBuffersCB new_buffers_cb_;
  NewMediaSegmentCB new_segment_cb_;
  base::Closure end_of_segment_cb_;
  LogCB log_cb_;

  ByteQueue queue_;

  AudioDecoderConfig config_;
  scoped_ptr<AudioTimestampHelper> timestamp_helper_;
  bool in_media_segment_;

  void ChangeState(State state);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int ParseFrameHeader(const uint8* data, int size,
                       int* frame_size,
                       int* sample_rate,
                       ChannelLayout* channel_layout,
                       int* sample_count) const;
  int ParseMP3Frame(const uint8* data, int size, BufferQueue* buffers);
  int ParseIcecastHeader(const uint8* data, int size);
  int ParseID3v1(const uint8* data, int size);
  int ParseID3v2(const uint8* data, int size);

  
  
  
  
  
  
  
  
  bool ParseSyncSafeInt(BitReader* reader, int32* value);

  
  
  
  
  
  
  int FindNextValidStartCode(const uint8* data, int size) const;

  
  
  
  
  
  
  bool SendBuffers(BufferQueue* buffers, bool end_of_segment);

  DISALLOW_COPY_AND_ASSIGN(MP3StreamParser);
};

}  

#endif  
