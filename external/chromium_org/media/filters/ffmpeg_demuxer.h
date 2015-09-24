// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_FILTERS_FFMPEG_DEMUXER_H_
#define MEDIA_FILTERS_FFMPEG_DEMUXER_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_vector.h"
#include "base/threading/thread.h"
#include "media/base/audio_decoder_config.h"
#include "media/base/decoder_buffer.h"
#include "media/base/decoder_buffer_queue.h"
#include "media/base/demuxer.h"
#include "media/base/pipeline.h"
#include "media/base/text_track_config.h"
#include "media/base/video_decoder_config.h"
#include "media/filters/blocking_url_protocol.h"

struct AVPacket;
struct AVRational;
struct AVStream;

namespace media {

class MediaLog;
class FFmpegDemuxer;
class FFmpegGlue;
class FFmpegH264ToAnnexBBitstreamConverter;
class ScopedPtrAVFreePacket;

typedef scoped_ptr_malloc<AVPacket, ScopedPtrAVFreePacket> ScopedAVPacket;

class FFmpegDemuxerStream : public DemuxerStream {
 public:
  
  
  FFmpegDemuxerStream(FFmpegDemuxer* demuxer, AVStream* stream);
  virtual ~FFmpegDemuxerStream();

  
  
  void EnqueuePacket(ScopedAVPacket packet);

  
  
  void SetEndOfStream();

  
  void FlushBuffers();

  
  void Stop();

  
  base::TimeDelta duration();

  
  virtual Type type() OVERRIDE;
  virtual void Read(const ReadCB& read_cb) OVERRIDE;
  virtual void EnableBitstreamConverter() OVERRIDE;
  virtual AudioDecoderConfig audio_decoder_config() OVERRIDE;
  virtual VideoDecoderConfig video_decoder_config() OVERRIDE;

  
  Ranges<base::TimeDelta> GetBufferedRanges() const;

  
  
  base::TimeDelta GetElapsedTime() const;

  
  bool HasAvailableCapacity();

  TextKind GetTextKind() const;

  
  
  std::string GetMetadata(const char* key) const;

 private:
  friend class FFmpegDemuxerTest;

  
  
  void SatisfyPendingRead();

  
  static base::TimeDelta ConvertStreamTimestamp(const AVRational& time_base,
                                                int64 timestamp);

  FFmpegDemuxer* demuxer_;
  scoped_refptr<base::MessageLoopProxy> message_loop_;
  AVStream* stream_;
  AudioDecoderConfig audio_config_;
  VideoDecoderConfig video_config_;
  Type type_;
  base::TimeDelta duration_;
  bool end_of_stream_;
  base::TimeDelta last_packet_timestamp_;
  Ranges<base::TimeDelta> buffered_ranges_;

  DecoderBufferQueue buffer_queue_;
  ReadCB read_cb_;

  scoped_ptr<FFmpegH264ToAnnexBBitstreamConverter> bitstream_converter_;
  bool bitstream_converter_enabled_;

  std::string encryption_key_id_;

  DISALLOW_COPY_AND_ASSIGN(FFmpegDemuxerStream);
};

class MEDIA_EXPORT FFmpegDemuxer : public Demuxer {
 public:
  FFmpegDemuxer(const scoped_refptr<base::MessageLoopProxy>& message_loop,
                DataSource* data_source,
                const NeedKeyCB& need_key_cb,
                const scoped_refptr<MediaLog>& media_log);
  virtual ~FFmpegDemuxer();

  
  virtual void Initialize(DemuxerHost* host,
                          const PipelineStatusCB& status_cb,
                          bool enable_text_tracks) OVERRIDE;
  virtual void Stop(const base::Closure& callback) OVERRIDE;
  virtual void Seek(base::TimeDelta time, const PipelineStatusCB& cb) OVERRIDE;
  virtual void OnAudioRendererDisabled() OVERRIDE;
  virtual DemuxerStream* GetStream(DemuxerStream::Type type) OVERRIDE;
  virtual base::TimeDelta GetStartTime() const OVERRIDE;

  
  void FireNeedKey(const std::string& init_data_type,
                   const std::string& encryption_key_id);

  
  
  void NotifyCapacityAvailable();
  void NotifyBufferingChanged();

 private:
  
  friend class FFmpegDemuxerTest;

  
  void OnOpenContextDone(const PipelineStatusCB& status_cb, bool result);
  void OnFindStreamInfoDone(const PipelineStatusCB& status_cb, int result);

  
  void OnSeekFrameDone(const PipelineStatusCB& cb, int result);

  
  void ReadFrameIfNeeded();
  void OnReadFrameDone(ScopedAVPacket packet, int result);

  
  void OnDataSourceStopped(const base::Closure& callback);

  
  
  bool StreamsHaveAvailableCapacity();

  
  void StreamHasEnded();

  
  void OnDataSourceError();

  
  
  FFmpegDemuxerStream* GetFFmpegStream(DemuxerStream::Type type) const;

  
  
  void AddTextStreams();

  DemuxerHost* host_;

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  base::WeakPtrFactory<FFmpegDemuxer> weak_factory_;
  base::WeakPtr<FFmpegDemuxer> weak_this_;

  
  base::Thread blocking_thread_;

  
  
  
  
  bool pending_read_;

  
  
  bool pending_seek_;

  
  
  
  
  
  
  
  
  
  typedef ScopedVector<FFmpegDemuxerStream> StreamVector;
  StreamVector streams_;

  
  
  DataSource* data_source_;

  scoped_refptr<MediaLog> media_log_;

  
  int bitrate_;

  
  
  
  base::TimeDelta start_time_;

  
  
  bool audio_disabled_;

  
  bool text_enabled_;

  
  
  bool duration_known_;

  
  BlockingUrlProtocol url_protocol_;
  scoped_ptr<FFmpegGlue> glue_;

  const NeedKeyCB need_key_cb_;

  DISALLOW_COPY_AND_ASSIGN(FFmpegDemuxer);
};

}  

#endif  
