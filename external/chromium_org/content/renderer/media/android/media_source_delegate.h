// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_ANDROID_MEDIA_SOURCE_DELEGATE_H_
#define CONTENT_RENDERER_MEDIA_ANDROID_MEDIA_SOURCE_DELEGATE_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/time/time.h"
#include "media/base/decryptor.h"
#include "media/base/demuxer.h"
#include "media/base/media_keys.h"
#include "media/base/pipeline_status.h"
#include "media/base/ranges.h"
#include "media/base/text_track.h"
#include "third_party/WebKit/public/platform/WebMediaPlayer.h"

namespace media {
class ChunkDemuxer;
class DecoderBuffer;
class DecryptingDemuxerStream;
class DemuxerStream;
class MediaLog;
struct DemuxerConfigs;
struct DemuxerData;
}

namespace content {

class RendererDemuxerAndroid;

class MediaSourceDelegate : public media::DemuxerHost {
 public:
  typedef base::Callback<void(blink::WebMediaSource*)>
      MediaSourceOpenedCB;
  typedef base::Callback<void(blink::WebMediaPlayer::NetworkState)>
      UpdateNetworkStateCB;
  typedef base::Callback<void(const base::TimeDelta&)> DurationChangeCB;

  
  
  class Destroyer {
   public:
    inline void operator()(void* media_source_delegate) const {
      static_cast<MediaSourceDelegate*>(media_source_delegate)->Destroy();
    }
  };

  MediaSourceDelegate(RendererDemuxerAndroid* demuxer_client,
                      int demuxer_client_id,
                      const scoped_refptr<base::MessageLoopProxy>& media_loop,
                      media::MediaLog* media_log);

  
  
  void InitializeMediaSource(
      const MediaSourceOpenedCB& media_source_opened_cb,
      const media::Demuxer::NeedKeyCB& need_key_cb,
      const media::SetDecryptorReadyCB& set_decryptor_ready_cb,
      const UpdateNetworkStateCB& update_network_state_cb,
      const DurationChangeCB& duration_change_cb);

#if defined(GOOGLE_TV)
  void InitializeMediaStream(
      media::Demuxer* demuxer,
      const UpdateNetworkStateCB& update_network_state_cb);
#endif

  const blink::WebTimeRanges& Buffered();
  size_t DecodedFrameCount() const;
  size_t DroppedFrameCount() const;
  size_t AudioDecodedByteCount() const;
  size_t VideoDecodedByteCount() const;

  
  
  
  void CancelPendingSeek(const base::TimeDelta& seek_time);

  
  
  
  
  
  void StartWaitingForSeek(const base::TimeDelta& seek_time);

  
  
  
  
  
  
  
  void Seek(const base::TimeDelta& seek_time, bool is_browser_seek);

  void NotifyKeyAdded(const std::string& key_system);

  
  void OnReadFromDemuxer(media::DemuxerStream::Type type);

  
  void OnMediaConfigRequest();

  
  void Destroy();

 private:
  typedef base::Callback<void(scoped_ptr<media::DemuxerData> data)>
      ReadFromDemuxerAckCB;
  typedef base::Callback<void(scoped_ptr<media::DemuxerConfigs> configs)>
      DemuxerReadyCB;

  
  virtual ~MediaSourceDelegate();

  
  virtual void SetTotalBytes(int64 total_bytes) OVERRIDE;
  virtual void AddBufferedByteRange(int64 start, int64 end) OVERRIDE;
  virtual void AddBufferedTimeRange(base::TimeDelta start,
                                    base::TimeDelta end) OVERRIDE;
  virtual void SetDuration(base::TimeDelta duration) OVERRIDE;
  virtual void OnDemuxerError(media::PipelineStatus status) OVERRIDE;
  virtual void AddTextStream(media::DemuxerStream* text_stream,
                             const media::TextTrackConfig& config) OVERRIDE;
  virtual void RemoveTextStream(media::DemuxerStream* text_stream) OVERRIDE;

  
  void OnDurationChanged(const base::TimeDelta& duration);

  
  void OnDemuxerInitDone(media::PipelineStatus status);

  
  void InitAudioDecryptingDemuxerStream();
  void InitVideoDecryptingDemuxerStream();

  
  void OnAudioDecryptingDemuxerStreamInitDone(media::PipelineStatus status);
  void OnVideoDecryptingDemuxerStreamInitDone(media::PipelineStatus status);

  
  
  
  
  void OnDemuxerSeekDone(media::PipelineStatus status);
  void ResetAudioDecryptingDemuxerStream();
  void ResetVideoDecryptingDemuxerStream();
  void FinishResettingDecryptingDemuxerStreams();

  void OnDemuxerStopDone();
  void OnDemuxerOpened();
  void OnNeedKey(const std::string& type,
                 const std::vector<uint8>& init_data);
  void NotifyDemuxerReady();
  bool CanNotifyDemuxerReady();

  void StopDemuxer();
  void InitializeDemuxer();
  void SeekInternal(const base::TimeDelta& seek_time);
  
  
  void ReadFromDemuxerStream(media::DemuxerStream::Type type,
                             scoped_ptr<media::DemuxerData> data,
                             size_t index);
  void OnBufferReady(media::DemuxerStream::Type type,
                     scoped_ptr<media::DemuxerData> data,
                     size_t index,
                     media::DemuxerStream::Status status,
                     const scoped_refptr<media::DecoderBuffer>& buffer);

  
  int GetDurationMs();

  bool HasEncryptedStream();

  bool IsSeeking() const;

  
  
  
  
  
  base::TimeDelta FindBufferedBrowserSeekTime_Locked(
      const base::TimeDelta& seek_time) const;

  
  const scoped_refptr<base::MessageLoopProxy> main_loop_;
  base::WeakPtrFactory<MediaSourceDelegate> main_weak_factory_;
  base::WeakPtr<MediaSourceDelegate> main_weak_this_;

  
  const scoped_refptr<base::MessageLoopProxy> media_loop_;
  base::WeakPtrFactory<MediaSourceDelegate> media_weak_factory_;

  RendererDemuxerAndroid* demuxer_client_;
  int demuxer_client_id_;

  scoped_refptr<media::MediaLog> media_log_;
  UpdateNetworkStateCB update_network_state_cb_;
  DurationChangeCB duration_change_cb_;

  scoped_ptr<media::ChunkDemuxer> chunk_demuxer_;
  media::Demuxer* demuxer_;
  bool is_demuxer_ready_;

  media::SetDecryptorReadyCB set_decryptor_ready_cb_;

  scoped_ptr<media::DecryptingDemuxerStream> audio_decrypting_demuxer_stream_;
  scoped_ptr<media::DecryptingDemuxerStream> video_decrypting_demuxer_stream_;

  media::DemuxerStream* audio_stream_;
  media::DemuxerStream* video_stream_;

  media::PipelineStatistics statistics_;
  media::Ranges<base::TimeDelta> buffered_time_ranges_;
  
  blink::WebTimeRanges buffered_web_time_ranges_;

  MediaSourceOpenedCB media_source_opened_cb_;
  media::Demuxer::NeedKeyCB need_key_cb_;

  
  
  blink::WebString current_key_system_;

  
  
  std::string init_data_type_;

  
  mutable base::Lock seeking_lock_;
  bool seeking_;

  
  
  
  
  
  bool doing_browser_seek_;
  base::TimeDelta browser_seek_time_;
  bool expecting_regular_seek_;

#if defined(GOOGLE_TV)
  bool key_added_;
  std::string key_system_;
#endif  

  size_t access_unit_size_;

  DISALLOW_COPY_AND_ASSIGN(MediaSourceDelegate);
};

}  

#endif  
