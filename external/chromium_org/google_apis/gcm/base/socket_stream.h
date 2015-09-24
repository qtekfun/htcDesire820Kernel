// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GCM_BASE_SOCKET_STREAM_H_
#define GOOGLE_APIS_GCM_BASE_SOCKET_STREAM_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "google_apis/gcm/base/gcm_export.h"
#include "net/base/net_errors.h"

namespace net {
class DrainableIOBuffer;
class IOBuffer;
class StreamSocket;
}  

namespace gcm {

class GCM_EXPORT SocketInputStream
    : public google::protobuf::io::ZeroCopyInputStream {
 public:
  enum State {
    
    
    EMPTY,
    
    READY,
    
    READING,
    
    CLOSED,
  };

  
  explicit SocketInputStream(net::StreamSocket* socket);
  virtual ~SocketInputStream();

  
  virtual bool Next(const void** data, int* size) OVERRIDE;
  virtual void BackUp(int count) OVERRIDE;
  virtual bool Skip(int count) OVERRIDE;  
  virtual int64 ByteCount() const OVERRIDE;

  
  size_t UnreadByteCount() const;

  
  
  
  
  
  
  
  net::Error Refresh(const base::Closure& callback, int byte_limit);

  
  
  
  
  void RebuildBuffer();

  
  
  net::Error last_error() const;

  
  State GetState() const;

 private:
  
  void ResetInternal();

  
  void RefreshCompletionCallback(const base::Closure& callback, int result);

  
  void CloseStream(net::Error error, const base::Closure& callback);

  
  net::StreamSocket* const socket_;
  const scoped_refptr<net::IOBuffer> io_buffer_;
  
  // been written to yet by Socket::Read calls.
  const scoped_refptr<net::DrainableIOBuffer> read_buffer_;

  
  
  
  int next_pos_;

  
  
  net::Error last_error_;

  base::WeakPtrFactory<SocketInputStream> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(SocketInputStream);
};

// 2. If EMPTY, the output stream can be written via a CodedOutputStream using
class GCM_EXPORT SocketOutputStream
    : public google::protobuf::io::ZeroCopyOutputStream {
 public:
  enum State {
    
    EMPTY,
    
    READY,
    
    FLUSHING,
    
    CLOSED,
  };

  
  explicit SocketOutputStream(net::StreamSocket* socket);
  virtual ~SocketOutputStream();

  
  virtual bool Next(void** data, int* size) OVERRIDE;
  virtual void BackUp(int count) OVERRIDE;
  virtual int64 ByteCount() const OVERRIDE;

  
  net::Error Flush(const base::Closure& callback);

  
  
  net::Error last_error() const;

  
  State GetState() const;

 private:
  void FlushCompletionCallback(const base::Closure& callback, int result);

  
  net::StreamSocket* const socket_;
  const scoped_refptr<net::IOBuffer> io_buffer_;
  
  // been written to the socket yet.
  const scoped_refptr<net::DrainableIOBuffer> write_buffer_;

  
  
  
  int next_pos_;

  
  
  net::Error last_error_;

  base::WeakPtrFactory<SocketOutputStream> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(SocketOutputStream);
};

}  

#endif  
