// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_ANDROID_MEDIA_DECODER_JOB_H_
#define MEDIA_BASE_ANDROID_MEDIA_DECODER_JOB_H_

#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "media/base/android/demuxer_stream_player_params.h"
#include "media/base/android/media_codec_bridge.h"

namespace base {
class MessageLoopProxy;
}

namespace media {

class MediaDecoderJob {
 public:
  struct Deleter {
    inline void operator()(MediaDecoderJob* ptr) const { ptr->Release(); }
  };

  
  
  
  
  
  typedef base::Callback<void(MediaCodecStatus, const base::TimeDelta&,
                              size_t)> DecoderCallback;
  
  
  typedef base::Callback<void(size_t)> ReleaseOutputCompletionCallback;

  virtual ~MediaDecoderJob();

  
  void OnDataReceived(const DemuxerData& data);

  
  
  void Prefetch(const base::Closure& prefetch_cb);

  
  
  
  
  
  
  
  bool Decode(const base::TimeTicks& start_time_ticks,
              const base::TimeDelta& start_presentation_timestamp,
              const DecoderCallback& callback);

  
  
  
  
  
  
  
  
  void StopDecode();

  
  void Flush();

  
  void BeginPrerolling(const base::TimeDelta& preroll_timestamp);

  bool prerolling() const { return prerolling_; }

  bool is_decoding() const { return !decode_cb_.is_null(); }

 protected:
  MediaDecoderJob(const scoped_refptr<base::MessageLoopProxy>& decoder_loop,
                  MediaCodecBridge* media_codec_bridge,
                  const base::Closure& request_data_cb);

  
  
  virtual void ReleaseOutputBuffer(
      int output_buffer_index,
      size_t size,
      bool render_output,
      const ReleaseOutputCompletionCallback& callback) = 0;

  
  
  virtual bool ComputeTimeToRender() const = 0;

 private:
  
  void Release();

  MediaCodecStatus QueueInputBuffer(const AccessUnit& unit);

  
  bool HasData() const;

  
  
  void RequestData(const base::Closure& done_cb);

  
  void DecodeNextAccessUnit(
      const base::TimeTicks& start_time_ticks,
      const base::TimeDelta& start_presentation_timestamp);

  
  
  
  
  
  
  void DecodeInternal(const AccessUnit& unit,
                      const base::TimeTicks& start_time_ticks,
                      const base::TimeDelta& start_presentation_timestamp,
                      bool needs_flush,
                      const DecoderCallback& callback);

  
  
  
  void OnDecodeCompleted(MediaCodecStatus status,
                         const base::TimeDelta& presentation_timestamp,
                         size_t audio_output_bytes);

  
  scoped_refptr<base::MessageLoopProxy> ui_loop_;

  
  scoped_refptr<base::MessageLoopProxy> decoder_loop_;

  
  
  MediaCodecBridge* media_codec_bridge_;

  
  bool needs_flush_;

  
  
  bool input_eos_encountered_;

  
  bool output_eos_encountered_;

  
  
  
  
  bool skip_eos_enqueue_;

  
  
  
  
  base::TimeDelta preroll_timestamp_;

  
  
  
  
  
  bool prerolling_;

  
  
  base::WeakPtrFactory<MediaDecoderJob> weak_this_;

  
  base::Closure request_data_cb_;

  
  base::Closure on_data_received_cb_;

  
  DecoderCallback decode_cb_;

  
  size_t access_unit_index_;

  
  DemuxerData received_data_;

  
  
  int input_buf_index_;

  bool stop_decode_pending_;

  
  
  
  bool destroy_pending_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(MediaDecoderJob);
};

}  

#endif  
