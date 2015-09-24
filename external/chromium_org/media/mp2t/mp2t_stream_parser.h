// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MP2T_MP2T_STREAM_PARSER_H_
#define MEDIA_MP2T_MP2T_STREAM_PARSER_H_

#include <list>
#include <map>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "media/base/audio_decoder_config.h"
#include "media/base/byte_queue.h"
#include "media/base/media_export.h"
#include "media/base/stream_parser.h"
#include "media/base/video_decoder_config.h"

namespace media {

class StreamParserBuffer;

namespace mp2t {

class PidState;

class MEDIA_EXPORT Mp2tStreamParser : public StreamParser {
 public:
  explicit Mp2tStreamParser(bool sbr_in_mimetype);
  virtual ~Mp2tStreamParser();

  
  virtual void Init(const InitCB& init_cb,
                    const NewConfigCB& config_cb,
                    const NewBuffersCB& new_buffers_cb,
                    const NewTextBuffersCB& text_cb,
                    const NeedKeyCB& need_key_cb,
                    const NewMediaSegmentCB& new_segment_cb,
                    const base::Closure& end_of_segment_cb,
                    const LogCB& log_cb) OVERRIDE;
  virtual void Flush() OVERRIDE;
  virtual bool Parse(const uint8* buf, int size) OVERRIDE;

 private:
  typedef std::map<int, PidState*> PidMap;

  struct BufferQueueWithConfig {
    BufferQueueWithConfig(bool is_cfg_sent,
                          const AudioDecoderConfig& audio_cfg,
                          const VideoDecoderConfig& video_cfg);
    ~BufferQueueWithConfig();

    bool is_config_sent;
    AudioDecoderConfig audio_config;
    StreamParser::BufferQueue audio_queue;
    VideoDecoderConfig video_config;
    StreamParser::BufferQueue video_queue;
  };

  
  
  void RegisterPmt(int program_number, int pmt_pid);

  
  
  
  
  void RegisterPes(int pmt_pid, int pes_pid, int stream_type);

  
  
  
  void UpdatePidFilter();

  
  
  void OnVideoConfigChanged(int pes_pid,
                            const VideoDecoderConfig& video_decoder_config);
  void OnAudioConfigChanged(int pes_pid,
                            const AudioDecoderConfig& audio_decoder_config);

  
  bool FinishInitializationIfNeeded();

  
  
  void OnEmitAudioBuffer(
      int pes_pid,
      scoped_refptr<StreamParserBuffer> stream_parser_buffer);
  void OnEmitVideoBuffer(
      int pes_pid,
      scoped_refptr<StreamParserBuffer> stream_parser_buffer);
  bool EmitRemainingBuffers();

  
  InitCB init_cb_;
  NewConfigCB config_cb_;
  NewBuffersCB new_buffers_cb_;
  NeedKeyCB need_key_cb_;
  NewMediaSegmentCB new_segment_cb_;
  base::Closure end_of_segment_cb_;
  LogCB log_cb_;

  
  
  bool sbr_in_mimetype_;

  
  ByteQueue ts_byte_queue_;

  
  PidMap pids_;

  
  int selected_audio_pid_;
  int selected_video_pid_;

  
  std::list<BufferQueueWithConfig> buffer_queue_chain_;

  
  bool is_initialized_;

  
  bool segment_started_;
  bool first_video_frame_in_segment_;
  base::TimeDelta time_offset_;

  DISALLOW_COPY_AND_ASSIGN(Mp2tStreamParser);
};

}  
}  

#endif

