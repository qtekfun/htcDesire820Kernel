// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_MEDIA_BUFFERED_RESOURCE_LOADER_H_
#define WEBKIT_GLUE_MEDIA_BUFFERED_RESOURCE_LOADER_H_

#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer.h"
#include "googleurl/src/gurl.h"
#include "media/base/seekable_buffer.h"
#include "net/base/file_stream.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebFrame.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebURLLoader.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebURLLoaderClient.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebURLRequest.h"
#include "webkit/glue/media/web_data_source.h"
#include "webkit/glue/webmediaplayer_impl.h"

namespace webkit_glue {

const int64 kPositionNotSpecified = -1;

const char kHttpScheme[] = "http";
const char kHttpsScheme[] = "https";
const char kDataScheme[] = "data";

class BufferedResourceLoader :
    public base::RefCountedThreadSafe<BufferedResourceLoader>,
    public WebKit::WebURLLoaderClient {
 public:
  
  
  
  enum DeferStrategy {
    kNeverDefer,
    kReadThenDefer,
    kThresholdDefer,
  };

  typedef Callback0::Type NetworkEventCallback;

  
  
  
  
  
  BufferedResourceLoader(const GURL& url,
                         int64 first_byte_position,
                         int64 last_byte_position);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Start(net::CompletionCallback* callback,
                     NetworkEventCallback* event_callback,
                     WebKit::WebFrame* frame);

  
  virtual void Stop();

  
  
  
  
  
  
  
  
  
  
  virtual void Read(int64 position, int read_size,
                    uint8* buffer, net::CompletionCallback* callback);

  
  
  virtual int64 GetBufferedPosition();

  
  
  
  virtual int64 content_length();

  
  
  virtual int64 instance_size();

  
  virtual bool range_supported();

  
  virtual bool network_activity();

  
  virtual const GURL& url();

  
  virtual void SetURLLoaderForTest(WebKit::WebURLLoader* mock_loader);

  
  virtual void willSendRequest(
      WebKit::WebURLLoader* loader,
      WebKit::WebURLRequest& newRequest,
      const WebKit::WebURLResponse& redirectResponse);
  virtual void didSendData(
      WebKit::WebURLLoader* loader,
      unsigned long long bytesSent,
      unsigned long long totalBytesToBeSent);
  virtual void didReceiveResponse(
      WebKit::WebURLLoader* loader,
      const WebKit::WebURLResponse& response);
  virtual void didDownloadData(
      WebKit::WebURLLoader* loader,
      int data_length);
  virtual void didReceiveData(
      WebKit::WebURLLoader* loader,
      const char* data,
      int data_length,
      int encoded_data_length);
  virtual void didReceiveCachedMetadata(
      WebKit::WebURLLoader* loader,
      const char* data, int dataLength);
  virtual void didFinishLoading(
      WebKit::WebURLLoader* loader,
      double finishTime);
  virtual void didFail(
      WebKit::WebURLLoader* loader,
      const WebKit::WebURLError&);

  bool HasSingleOrigin() const;

  
  void UpdateDeferStrategy(DeferStrategy strategy);

 protected:
  friend class base::RefCountedThreadSafe<BufferedResourceLoader>;
  virtual ~BufferedResourceLoader();

 private:
  friend class BufferedResourceLoaderTest;

  
  
  bool ToggleDeferring();

  
  
  bool ShouldEnableDefer();

  
  
  bool ShouldDisableDefer();

  
  void UpdateDeferBehavior();

  
  
  bool CanFulfillRead();

  
  bool WillFulfillRead();

  
  
  void ReadInternal();

  
  bool VerifyPartialResponse(const WebKit::WebURLResponse& response);

  
  
  
  
  
  
  std::string GenerateHeaders(int64 first_byte_position,
                              int64 last_byte_position);

  
  
  void DoneRead(int error);

  
  void DoneStart(int error);

  
  void NotifyNetworkEvent();

  bool HasPendingRead() { return read_callback_.get() != NULL; }

  
  bool IsRangeRequest() const;

  
  scoped_ptr<media::SeekableBuffer> buffer_;

  
  bool deferred_;

  
  DeferStrategy defer_strategy_;

  
  bool completed_;

  
  bool range_requested_;

  
  bool range_supported_;

  
  scoped_ptr<WebKit::WebURLLoader> url_loader_;

  GURL url_;
  int64 first_byte_position_;
  int64 last_byte_position_;
  bool single_origin_;

  
  scoped_ptr<NetworkEventCallback> event_callback_;

  
  scoped_ptr<net::CompletionCallback> start_callback_;
  int64 offset_;
  int64 content_length_;
  int64 instance_size_;

  
  
  scoped_ptr<net::CompletionCallback> read_callback_;
  int64 read_position_;
  int read_size_;
  uint8* read_buffer_;

  
  // written by Read().
  int first_offset_;
  int last_offset_;

  
  bool keep_test_loader_;

  DISALLOW_COPY_AND_ASSIGN(BufferedResourceLoader);
};

}  

#endif  
