// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_STREAM_TEST_UTIL_H_
#define NET_SPDY_SPDY_STREAM_TEST_UTIL_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "net/base/io_buffer.h"
#include "net/base/test_completion_callback.h"
#include "net/spdy/spdy_read_queue.h"
#include "net/spdy/spdy_stream.h"

namespace net {

namespace test {

class ClosingDelegate : public SpdyStream::Delegate {
 public:
  explicit ClosingDelegate(const base::WeakPtr<SpdyStream>& stream);
  virtual ~ClosingDelegate();

  
  virtual void OnRequestHeadersSent() OVERRIDE;
  virtual SpdyResponseHeadersStatus OnResponseHeadersUpdated(
      const SpdyHeaderBlock& response_headers) OVERRIDE;
  virtual void OnDataReceived(scoped_ptr<SpdyBuffer> buffer) OVERRIDE;
  virtual void OnDataSent() OVERRIDE;
  virtual void OnClose(int status) OVERRIDE;

  
  bool StreamIsClosed() const { return !stream_.get(); }

 private:
  base::WeakPtr<SpdyStream> stream_;
};

class StreamDelegateBase : public SpdyStream::Delegate {
 public:
  explicit StreamDelegateBase(const base::WeakPtr<SpdyStream>& stream);
  virtual ~StreamDelegateBase();

  virtual void OnRequestHeadersSent() OVERRIDE;
  virtual SpdyResponseHeadersStatus OnResponseHeadersUpdated(
      const SpdyHeaderBlock& response_headers) OVERRIDE;
  virtual void OnDataReceived(scoped_ptr<SpdyBuffer> buffer) OVERRIDE;
  virtual void OnDataSent() OVERRIDE;
  virtual void OnClose(int status) OVERRIDE;

  
  
  int WaitForClose();

  
  
  std::string TakeReceivedData();

  
  bool StreamIsClosed() const { return !stream_.get(); }

  
  
  SpdyStreamId stream_id() const { return stream_id_; }

  std::string GetResponseHeaderValue(const std::string& name) const;
  bool send_headers_completed() const { return send_headers_completed_; }

 protected:
  const base::WeakPtr<SpdyStream>& stream() { return stream_; }

 private:
  base::WeakPtr<SpdyStream> stream_;
  SpdyStreamId stream_id_;
  TestCompletionCallback callback_;
  bool send_headers_completed_;
  SpdyHeaderBlock response_headers_;
  SpdyReadQueue received_data_queue_;
};

class StreamDelegateDoNothing : public StreamDelegateBase {
 public:
  StreamDelegateDoNothing(const base::WeakPtr<SpdyStream>& stream);
  virtual ~StreamDelegateDoNothing();
};

class StreamDelegateSendImmediate : public StreamDelegateBase {
 public:
  
  StreamDelegateSendImmediate(const base::WeakPtr<SpdyStream>& stream,
                              base::StringPiece data);
  virtual ~StreamDelegateSendImmediate();

  virtual SpdyResponseHeadersStatus OnResponseHeadersUpdated(
      const SpdyHeaderBlock& response_headers) OVERRIDE;

 private:
  base::StringPiece data_;
};

class StreamDelegateWithBody : public StreamDelegateBase {
 public:
  StreamDelegateWithBody(const base::WeakPtr<SpdyStream>& stream,
                         base::StringPiece data);
  virtual ~StreamDelegateWithBody();

  virtual void OnRequestHeadersSent() OVERRIDE;

 private:
  scoped_refptr<StringIOBuffer> buf_;
};

} 

} 

#endif 
