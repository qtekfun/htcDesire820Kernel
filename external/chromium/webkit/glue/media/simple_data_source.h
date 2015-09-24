// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef WEBKIT_GLUE_MEDIA_SIMPLE_DATA_SOURCE_H_
#define WEBKIT_GLUE_MEDIA_SIMPLE_DATA_SOURCE_H_

#include <algorithm>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "media/base/filter_factories.h"
#include "media/base/filters.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebFrame.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebURLLoader.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebURLLoaderClient.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebURLRequest.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebURLResponse.h"
#include "webkit/glue/media/web_data_source.h"

class MessageLoop;
class WebMediaPlayerDelegateImpl;

namespace webkit_glue {

class SimpleDataSource : public WebDataSource,
                         public WebKit::WebURLLoaderClient {
 public:
  
  static media::DataSourceFactory* CreateFactory(
      MessageLoop* render_loop,
      WebKit::WebFrame* frame,
      WebDataSourceBuildObserverHack* build_observer);

  SimpleDataSource(MessageLoop* render_loop, WebKit::WebFrame* frame);
  virtual ~SimpleDataSource();

  
  virtual void set_host(media::FilterHost* host);
  virtual void Stop(media::FilterCallback* callback);

  
  virtual const media::MediaFormat& media_format();
  virtual void Read(int64 position, size_t size,
                    uint8* data, ReadCallback* read_callback);
  virtual bool GetSize(int64* size_out);
  virtual bool IsStreaming();
  virtual void SetPreload(media::Preload preload);

  
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
      int dataLength);
  virtual void didReceiveData(
      WebKit::WebURLLoader* loader,
      const char* data,
      int dataLength,
      int encodedDataLength);
  virtual void didReceiveCachedMetadata(
      WebKit::WebURLLoader* loader,
      const char* data, int dataLength);
  virtual void didFinishLoading(
      WebKit::WebURLLoader* loader,
      double finishTime);
  virtual void didFail(
      WebKit::WebURLLoader* loader,
      const WebKit::WebURLError&);

  
  virtual void Initialize(const std::string& url,
                          media::PipelineStatusCallback* callback);
  virtual void CancelInitialize();
  virtual bool HasSingleOrigin();
  virtual void Abort();

 private:
  
  void SetURL(const GURL& url);

  
  void StartTask();

  
  void CancelTask();

  
  void DoneInitialization_Locked(bool success);

  
  void UpdateHostState();

  
  MessageLoop* render_loop_;

  
  WebKit::WebFrame* frame_;

  
  scoped_ptr<WebKit::WebURLLoader> url_loader_;

  media::MediaFormat media_format_;
  GURL url_;
  std::string data_;
  int64 size_;
  bool single_origin_;

  
  enum State {
    UNINITIALIZED,
    INITIALIZING,
    INITIALIZED,
    STOPPED,
  };
  State state_;

  
  base::Lock lock_;

  
  scoped_ptr<media::PipelineStatusCallback> initialize_callback_;

  
  bool keep_test_loader_;

  DISALLOW_COPY_AND_ASSIGN(SimpleDataSource);
};

}  

#endif  
