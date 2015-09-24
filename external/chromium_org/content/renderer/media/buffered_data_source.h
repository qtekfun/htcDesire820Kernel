// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_BUFFERED_DATA_SOURCE_H_
#define CONTENT_RENDERER_MEDIA_BUFFERED_DATA_SOURCE_H_

#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "content/common/content_export.h"
#include "content/renderer/media/buffered_resource_loader.h"
#include "content/renderer/media/preload.h"
#include "media/base/data_source.h"
#include "media/base/ranges.h"
#include "url/gurl.h"

namespace base {
class MessageLoopProxy;
}

namespace media {
class MediaLog;
}

namespace content {

class CONTENT_EXPORT BufferedDataSource : public media::DataSource {
 public:
  typedef base::Callback<void(bool)> DownloadingCB;

  
  
  BufferedDataSource(const scoped_refptr<base::MessageLoopProxy>& render_loop,
                     blink::WebFrame* frame,
                     media::MediaLog* media_log,
                     const DownloadingCB& downloading_cb);
  virtual ~BufferedDataSource();

  
  
  
  
  typedef base::Callback<void(bool)> InitializeCB;
  void Initialize(
      const GURL& url,
      BufferedResourceLoader::CORSMode cors_mode,
      const InitializeCB& init_cb);

  
  void SetPreload(Preload preload);

  
  
  
  
  bool HasSingleOrigin();

  
  bool DidPassCORSAccessCheck() const;

  
  
  
  
  
  void Abort();

  
  
  void MediaPlaybackRateChanged(float playback_rate);
  void MediaIsPlaying();
  void MediaIsPaused();

  
  
  virtual void set_host(media::DataSourceHost* host) OVERRIDE;
  virtual void Stop(const base::Closure& closure) OVERRIDE;

  virtual void Read(int64 position, int size, uint8* data,
                    const media::DataSource::ReadCB& read_cb) OVERRIDE;
  virtual bool GetSize(int64* size_out) OVERRIDE;
  virtual bool IsStreaming() OVERRIDE;
  virtual void SetBitrate(int bitrate) OVERRIDE;

 protected:
  
  
  
  virtual BufferedResourceLoader* CreateResourceLoader(
      int64 first_byte_position, int64 last_byte_position);

 private:
  friend class BufferedDataSourceTest;

  
  void ReadTask();

  
  
  void StopInternal_Locked();

  
  void StopLoader();

  
  void SetBitrateTask(int bitrate);

  
  
  void ReadInternal();

  
  void StartCallback(BufferedResourceLoader::Status status);

  
  
  void PartialReadStartCallback(BufferedResourceLoader::Status status);

  
  void ReadCallback(BufferedResourceLoader::Status status, int bytes_read);
  void LoadingStateChangedCallback(BufferedResourceLoader::LoadingState state);
  void ProgressCallback(int64 position);

  
  
  void ReportOrQueueBufferedBytes(int64 start, int64 end);

  void UpdateHostState_Locked();

  
  
  void UpdateDeferStrategy(bool paused);

  base::WeakPtr<BufferedDataSource> weak_this_;

  
  GURL url_;
  
  BufferedResourceLoader::CORSMode cors_mode_;

  
  
  
  int64 total_bytes_;

  
  
  bool assume_fully_buffered_;

  
  
  bool streaming_;

  
  blink::WebFrame* frame_;

  
  scoped_ptr<BufferedResourceLoader> loader_;

  
  InitializeCB init_cb_;

  
  
  class ReadOperation;
  scoped_ptr<ReadOperation> read_op_;

  
  
  
  
  
  
  
  
  
  
  scoped_ptr<uint8[]> intermediate_read_buffer_;
  int intermediate_read_buffer_size_;

  
  const scoped_refptr<base::MessageLoopProxy> render_loop_;

  
  base::Lock lock_;

  
  bool stop_signal_received_;

  
  
  bool media_has_played_;

  
  
  Preload preload_;

  
  int bitrate_;

  
  float playback_rate_;

  
  media::Ranges<int64> queued_buffered_byte_ranges_;

  scoped_refptr<media::MediaLog> media_log_;

  DownloadingCB downloading_cb_;

  base::WeakPtrFactory<BufferedDataSource> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(BufferedDataSource);
};

}  

#endif  
