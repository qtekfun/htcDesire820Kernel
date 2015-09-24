// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_MEDIA_BUFFERED_DATA_SOURCE_H_
#define WEBKIT_GLUE_MEDIA_BUFFERED_DATA_SOURCE_H_

#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "media/base/filter_factories.h"
#include "media/base/filters.h"
#include "webkit/glue/media/buffered_resource_loader.h"

namespace webkit_glue {

class BufferedDataSource : public WebDataSource {
 public:
  
  static media::DataSourceFactory* CreateFactory(
      MessageLoop* render_loop,
      WebKit::WebFrame* frame,
      WebDataSourceBuildObserverHack* build_observer);

  BufferedDataSource(MessageLoop* render_loop,
                     WebKit::WebFrame* frame);

  virtual ~BufferedDataSource();

  
  virtual void set_host(media::FilterHost* host);
  virtual void Stop(media::FilterCallback* callback);
  virtual void SetPlaybackRate(float playback_rate);

  
  
  virtual void Read(int64 position, size_t size,
                    uint8* data,
                    media::DataSource::ReadCallback* read_callback);
  virtual bool GetSize(int64* size_out);
  virtual bool IsStreaming();
  virtual void SetPreload(media::Preload preload);

  const media::MediaFormat& media_format() {
    return media_format_;
  }

  
  virtual void Initialize(const std::string& url,
                          media::PipelineStatusCallback* callback);
  virtual void CancelInitialize();
  virtual bool HasSingleOrigin();
  virtual void Abort();

 protected:
  
  
  
  virtual BufferedResourceLoader* CreateResourceLoader(
      int64 first_byte_position, int64 last_byte_position);

  
  
  
  virtual base::TimeDelta GetTimeoutMilliseconds();

 private:
  
  
  void InitializeTask();

  
  void ReadTask(int64 position, int read_size, uint8* read_buffer);

  
  
  
  void CleanupTask();

  
  void RestartLoadingTask();

  
  
  
  void WatchDogTask();

  
  
  
  void SetPlaybackRateTask(float playback_rate);

  
  void SetPreloadTask(media::Preload preload);

  
  
  BufferedResourceLoader::DeferStrategy ChooseDeferStrategy();

  
  
  void ReadInternal();

  
  void DoneRead_Locked(int error);

  
  void DoneInitialization_Locked(media::PipelineStatus status);

  
  
  
  void HttpInitialStartCallback(int error);

  
  
  
  void NonHttpInitialStartCallback(int error);

  
  
  
  
  void PartialReadStartCallback(int error);

  
  
  
  void ReadCallback(int error);

  
  void NetworkEventCallback();

  void UpdateHostState();

  media::MediaFormat media_format_;

  
  GURL url_;

  // Members for total bytes of the requested object. It is written once on
  
  // member is guaranteed to happen after it is first written, so we don't
  
  int64 total_bytes_;
  int64 buffered_bytes_;

  
  bool loaded_;

  
  
  bool streaming_;

  
  WebKit::WebFrame* frame_;

  
  scoped_refptr<BufferedResourceLoader> loader_;

  
  bool network_activity_;

  
  scoped_ptr<media::PipelineStatusCallback> initialize_callback_;

  
  scoped_ptr<media::DataSource::ReadCallback> read_callback_;
  int64 read_position_;
  int read_size_;
  uint8* read_buffer_;
  base::Time read_submitted_time_;
  int read_attempts_;

  
  
  
  
  
  
  
  
  
  
  scoped_array<uint8> intermediate_read_buffer_;
  int intermediate_read_buffer_size_;

  
  MessageLoop* render_loop_;

  
  base::Lock lock_;

  
  
  bool stop_signal_received_;

  
  
  bool stopped_on_render_loop_;

  
  
  bool media_is_paused_;

  
  
  bool media_has_played_;

  
  
  media::Preload preload_;

  
  
  
  
  base::RepeatingTimer<BufferedDataSource> watch_dog_timer_;

  
  
  bool using_range_request_;

  DISALLOW_COPY_AND_ASSIGN(BufferedDataSource);
};

}  

#endif  
