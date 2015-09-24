// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_CHUNK_DEMUXER_H_
#define MEDIA_FILTERS_CHUNK_DEMUXER_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/synchronization/lock.h"
#include "media/base/byte_queue.h"
#include "media/base/demuxer.h"
#include "media/base/ranges.h"
#include "media/base/stream_parser.h"
#include "media/filters/source_buffer_stream.h"

namespace media {

class ChunkDemuxerStream;
class FFmpegURLProtocol;
class SourceState;

class MEDIA_EXPORT ChunkDemuxer : public Demuxer {
 public:
  enum Status {
    kOk,              
    kNotSupported,    
    kReachedIdLimit,  
  };

  
  
  
  
  
  
  
  
  ChunkDemuxer(const base::Closure& open_cb,
               const NeedKeyCB& need_key_cb,
               const LogCB& log_cb);
  virtual ~ChunkDemuxer();

  
  virtual void Initialize(DemuxerHost* host,
                          const PipelineStatusCB& cb,
                          bool enable_text_tracks) OVERRIDE;
  virtual void Stop(const base::Closure& callback) OVERRIDE;
  virtual void Seek(base::TimeDelta time, const PipelineStatusCB&  cb) OVERRIDE;
  virtual void OnAudioRendererDisabled() OVERRIDE;
  virtual DemuxerStream* GetStream(DemuxerStream::Type type) OVERRIDE;
  virtual base::TimeDelta GetStartTime() const OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  void StartWaitingForSeek(base::TimeDelta seek_time);

  
  
  
  
  
  
  
  
  
  
  void CancelPendingSeek(base::TimeDelta seek_time);

  
  
  
  
  
  
  
  Status AddId(const std::string& id, const std::string& type,
               std::vector<std::string>& codecs);

  
  
  void RemoveId(const std::string& id);

  
  Ranges<base::TimeDelta> GetBufferedRanges(const std::string& id) const;

  
  void AppendData(const std::string& id, const uint8* data, size_t length);

  
  
  void Abort(const std::string& id);

  
  
  void Remove(const std::string& id, base::TimeDelta start,
              base::TimeDelta end);

  
  double GetDuration();
  double GetDuration_Locked();

  
  
  void SetDuration(double duration);

  
  
  
  
  bool SetTimestampOffset(const std::string& id, base::TimeDelta offset);

  
  
  
  void MarkEndOfStream(PipelineStatus status);
  void UnmarkEndOfStream();

  
  
  void SetAppendWindowStart(const std::string& id, base::TimeDelta start);
  void SetAppendWindowEnd(const std::string& id, base::TimeDelta end);

  void Shutdown();

  void SetMemoryLimitsForTesting(int memory_limit);

  
  
  
  
  Ranges<base::TimeDelta> GetBufferedRanges() const;

 private:
  enum State {
    WAITING_FOR_INIT,
    INITIALIZING,
    INITIALIZED,
    ENDED,
    PARSE_ERROR,
    SHUTDOWN,
  };

  void ChangeState_Locked(State new_state);

  
  
  void ReportError_Locked(PipelineStatus error);

  
  bool IsSeekWaitingForData_Locked() const;

  
  
  bool CanEndOfStream_Locked() const;

  
  void OnSourceInitDone(bool success, base::TimeDelta duration);

  
  
  
  ChunkDemuxerStream* CreateDemuxerStream(DemuxerStream::Type type);

  void OnNewTextTrack(ChunkDemuxerStream* text_stream,
                      const TextTrackConfig& config);
  void OnNewMediaSegment(const std::string& source_id,
                         base::TimeDelta start_timestamp);

  
  
  Ranges<base::TimeDelta> ComputeIntersection() const;

  
  void AdjustBufferTimestamps(const StreamParser::BufferQueue& buffers,
                              base::TimeDelta timestamp_offset);

  
  bool IsValidId(const std::string& source_id) const;

  
  
  
  void IncreaseDurationIfNecessary(
      base::TimeDelta last_appended_buffer_timestamp,
      ChunkDemuxerStream* stream);

  
  
  void DecreaseDurationIfNecessary();

  
  
  void UpdateDuration(base::TimeDelta new_duration);

  
  Ranges<base::TimeDelta> GetBufferedRanges_Locked() const;

  
  void StartReturningData();

  
  void AbortPendingReads();

  
  void CompletePendingReadsIfPossible();

  
  void SeekAllSources(base::TimeDelta seek_time);

  mutable base::Lock lock_;
  State state_;
  bool cancel_next_seek_;

  DemuxerHost* host_;
  base::Closure open_cb_;
  NeedKeyCB need_key_cb_;
  bool enable_text_;
  
  
  LogCB log_cb_;

  PipelineStatusCB init_cb_;
  
  
  
  
  PipelineStatusCB seek_cb_;

  scoped_ptr<ChunkDemuxerStream> audio_;
  scoped_ptr<ChunkDemuxerStream> video_;

  
  scoped_ptr<ChunkDemuxerStream> disabled_audio_;

  base::TimeDelta duration_;

  
  
  
  
  
  double user_specified_duration_;

  typedef std::map<std::string, SourceState*> SourceStateMap;
  SourceStateMap source_state_map_;

  
  
  
  std::string source_id_audio_;
  std::string source_id_video_;

  DISALLOW_COPY_AND_ASSIGN(ChunkDemuxer);
};

}  

#endif  
