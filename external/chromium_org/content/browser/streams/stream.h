// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_STREAMS_STREAM_H_
#define CONTENT_BROWSER_STREAMS_STREAM_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "content/browser/byte_stream.h"
#include "content/common/content_export.h"
#include "url/gurl.h"

namespace net {
class IOBuffer;
}

namespace content {

class StreamHandle;
class StreamHandleImpl;
class StreamReadObserver;
class StreamRegistry;
class StreamWriteObserver;

class CONTENT_EXPORT Stream : public base::RefCountedThreadSafe<Stream> {
 public:
  enum StreamState {
    STREAM_HAS_DATA,
    STREAM_COMPLETE,
    STREAM_EMPTY,
    STREAM_ABORTED,
  };

  
  
  
  
  
  Stream(StreamRegistry* registry,
         StreamWriteObserver* write_observer,
         const GURL& url);

  
  
  bool SetReadObserver(StreamReadObserver* observer);

  
  void RemoveReadObserver(StreamReadObserver* observer);

  
  void RemoveWriteObserver(StreamWriteObserver* observer);

  
  
  void Abort();

  
  void AddData(scoped_refptr<net::IOBuffer> buffer, size_t size);
  
  
  void AddData(const char* data, size_t size);

  
  void Finalize();

  
  
  
  
  StreamState ReadRawData(net::IOBuffer* buf, int buf_size, int* bytes_read);

  scoped_ptr<StreamHandle> CreateHandle(const GURL& original_url,
                                        const std::string& mime_type);
  void CloseHandle();

  
  bool can_add_data() const { return can_add_data_; }

  const GURL& url() const { return url_; }

  
  size_t last_total_buffered_bytes() const {
    return last_total_buffered_bytes_;
  }

 private:
  friend class base::RefCountedThreadSafe<Stream>;

  virtual ~Stream();

  void OnSpaceAvailable();
  void OnDataAvailable();

  
  void ClearBuffer();

  bool can_add_data_;

  GURL url_;

  
  
  scoped_refptr<net::IOBuffer> data_;
  
  
  size_t data_length_;
  
  size_t data_bytes_read_;

  
  
  size_t last_total_buffered_bytes_;

  scoped_ptr<ByteStreamWriter> writer_;
  scoped_ptr<ByteStreamReader> reader_;

  StreamRegistry* registry_;
  StreamReadObserver* read_observer_;
  StreamWriteObserver* write_observer_;

  StreamHandleImpl* stream_handle_;

  base::WeakPtrFactory<Stream> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(Stream);
};

}  

#endif  
