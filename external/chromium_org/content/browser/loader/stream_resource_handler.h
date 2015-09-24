// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_LOADER_STREAM_RESOURCE_HANDLER_H_
#define CONTENT_BROWSER_LOADER_STREAM_RESOURCE_HANDLER_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/browser/loader/resource_handler.h"
#include "content/browser/streams/stream_write_observer.h"
#include "url/gurl.h"

namespace net {
class URLRequest;
}  

namespace content {

class StreamRegistry;

class StreamResourceHandler : public StreamWriteObserver,
                              public ResourceHandler {
 public:
  
  
  
  StreamResourceHandler(net::URLRequest* request,
                        StreamRegistry* registry,
                        const GURL& origin);
  virtual ~StreamResourceHandler();

  virtual bool OnUploadProgress(int request_id,
                                uint64 position,
                                uint64 size) OVERRIDE;

  
  virtual bool OnRequestRedirected(int request_id,
                                   const GURL& url,
                                   ResourceResponse* resp,
                                   bool* defer) OVERRIDE;

  virtual bool OnResponseStarted(int request_id,
                                 ResourceResponse* resp,
                                 bool* defer) OVERRIDE;

  virtual bool OnWillStart(int request_id,
                           const GURL& url,
                           bool* defer) OVERRIDE;

  
  virtual bool OnWillRead(int request_id,
                          scoped_refptr<net::IOBuffer>* buf,
                          int* buf_size,
                          int min_size) OVERRIDE;

  
  virtual bool OnReadCompleted(int request_id,
                               int bytes_read,
                               bool* defer) OVERRIDE;

  virtual void OnResponseCompleted(int request_id,
                                   const net::URLRequestStatus& status,
                                   const std::string& sec_info,
                                   bool* defer) OVERRIDE;

  virtual void OnDataDownloaded(int request_id, int bytes_downloaded) OVERRIDE;

  Stream* stream() { return stream_.get(); }

 private:
  virtual void OnSpaceAvailable(Stream* stream) OVERRIDE;
  virtual void OnClose(Stream* stream) OVERRIDE;

  scoped_refptr<Stream> stream_;
  scoped_refptr<net::IOBuffer> read_buffer_;
  DISALLOW_COPY_AND_ASSIGN(StreamResourceHandler);
};

}  

#endif  
