// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_BUFFERED_RESOURCE_LOADER_H_
#define CONTENT_RENDERER_MEDIA_BUFFERED_RESOURCE_LOADER_H_

#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "content/common/content_export.h"
#include "content/renderer/media/active_loader.h"
#include "media/base/seekable_buffer.h"
#include "third_party/WebKit/public/platform/WebURLLoader.h"
#include "third_party/WebKit/public/platform/WebURLLoaderClient.h"
#include "third_party/WebKit/public/platform/WebURLRequest.h"
#include "third_party/WebKit/public/web/WebFrame.h"
#include "url/gurl.h"

namespace media {
class MediaLog;
class SeekableBuffer;
}

namespace content {

const int64 kPositionNotSpecified = -1;

const char kHttpScheme[] = "http";
const char kHttpsScheme[] = "https";

class CONTENT_EXPORT BufferedResourceLoader
    : NON_EXPORTED_BASE(public blink::WebURLLoaderClient) {
 public:
  
  
  
  enum DeferStrategy {
    kNeverDefer,
    kReadThenDefer,
    kCapacityDefer,
  };

  
  enum Status {
    
    kOk,

    
    
    
    
    
    
    
    kFailed,

    
    
    kCacheMiss,
  };

  
  enum CORSMode { kUnspecified, kAnonymous, kUseCredentials };

  enum LoadingState {
    kLoading,  
    kLoadingDeferred,  
    kLoadingFinished,  
    kLoadingFailed,  
  };

  
  
  
  
  
  
  
  
  
  BufferedResourceLoader(
      const GURL& url,
      CORSMode cors_mode,
      int64 first_byte_position,
      int64 last_byte_position,
      DeferStrategy strategy,
      int bitrate,
      float playback_rate,
      media::MediaLog* media_log);
  virtual ~BufferedResourceLoader();

  
  
  
  
  typedef base::Callback<void(Status)> StartCB;
  typedef base::Callback<void(LoadingState)> LoadingStateChangedCB;
  typedef base::Callback<void(int64)> ProgressCB;
  void Start(const StartCB& start_cb,
             const LoadingStateChangedCB& loading_cb,
             const ProgressCB& progress_cb,
             blink::WebFrame* frame);

  
  
  
  
  void Stop();

  
  
  
  
  
  
  
  
  typedef base::Callback<void(Status, int)> ReadCB;
  void Read(int64 position, int read_size,
            uint8* buffer, const ReadCB& read_cb);

  
  
  
  int64 content_length();

  
  
  int64 instance_size();

  
  bool range_supported();

  
  virtual void willSendRequest(
      blink::WebURLLoader* loader,
      blink::WebURLRequest& newRequest,
      const blink::WebURLResponse& redirectResponse);
  virtual void didSendData(
      blink::WebURLLoader* loader,
      unsigned long long bytesSent,
      unsigned long long totalBytesToBeSent);
  virtual void didReceiveResponse(
      blink::WebURLLoader* loader,
      const blink::WebURLResponse& response);
  virtual void didDownloadData(
      blink::WebURLLoader* loader,
      int data_length,
      int encoded_data_length);
  virtual void didReceiveData(
      blink::WebURLLoader* loader,
      const char* data,
      int data_length,
      int encoded_data_length);
  virtual void didReceiveCachedMetadata(
      blink::WebURLLoader* loader,
      const char* data, int dataLength);
  virtual void didFinishLoading(
      blink::WebURLLoader* loader,
      double finishTime);
  virtual void didFail(
      blink::WebURLLoader* loader,
      const blink::WebURLError&);

  
  
  bool HasSingleOrigin() const;

  
  
  bool DidPassCORSAccessCheck() const;

  
  
  
  
  void UpdateDeferStrategy(DeferStrategy strategy);

  
  
  void SetPlaybackRate(float playback_rate);

  
  
  void SetBitrate(int bitrate);

  
  int64 first_byte_position() const;

  
  
  
  
  
  
  static bool ParseContentRange(
      const std::string& content_range_str, int64* first_byte_position,
      int64* last_byte_position, int64* instance_size);

 private:
  friend class BufferedDataSourceTest;
  friend class BufferedResourceLoaderTest;
  friend class MockBufferedDataSource;

  
  void UpdateBufferWindow();

  
  void UpdateDeferBehavior();

  
  
  void SetDeferred(bool deferred);

  
  
  bool ShouldDefer() const;

  
  
  bool CanFulfillRead() const;

  
  bool WillFulfillRead() const;

  
  
  void ReadInternal();

  
  bool VerifyPartialResponse(const blink::WebURLResponse& response);

  
  
  void DoneRead(Status status, int bytes_read);

  
  void DoneStart(Status status);

  bool HasPendingRead() { return !read_cb_.is_null(); }

  
  bool IsRangeRequest() const;

  
  void Log();

  
  media::SeekableBuffer buffer_;

  
  scoped_ptr<ActiveLoader> active_loader_;

  
  
  bool loader_failed_;

  
  DeferStrategy defer_strategy_;

  
  
  bool might_be_reused_from_cache_in_future_;

  
  bool range_supported_;

  
  size_t saved_forward_capacity_;

  GURL url_;
  CORSMode cors_mode_;
  const int64 first_byte_position_;
  const int64 last_byte_position_;
  bool single_origin_;

  
  LoadingStateChangedCB loading_cb_;

  
  
  ProgressCB progress_cb_;

  
  StartCB start_cb_;
  int64 offset_;
  int64 content_length_;
  int64 instance_size_;

  
  
  ReadCB read_cb_;
  int64 read_position_;
  int read_size_;
  uint8* read_buffer_;

  
  // written by Read().
  int first_offset_;
  int last_offset_;

  
  scoped_ptr<blink::WebURLLoader> test_loader_;

  
  int bitrate_;

  
  float playback_rate_;

  scoped_refptr<media::MediaLog> media_log_;

  DISALLOW_COPY_AND_ASSIGN(BufferedResourceLoader);
};

}  

#endif  
