// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_BUFFERED_SOCKET_WRITER_H_
#define REMOTING_PROTOCOL_BUFFERED_SOCKET_WRITER_H_

#include <list>

#include "base/callback.h"
#include "base/synchronization/lock.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/io_buffer.h"
#include "net/socket/socket.h"

namespace net {
class Socket;
}  

namespace remoting {
namespace protocol {

// Whenever new data is written it is just put in the queue, and then written

class BufferedSocketWriterBase : public base::NonThreadSafe {
 public:
  typedef base::Callback<void(int)> WriteFailedCallback;

  BufferedSocketWriterBase();
  virtual ~BufferedSocketWriterBase();

  
  
  
  
  void Init(net::Socket* socket, const WriteFailedCallback& callback);

  
  
  bool Write(scoped_refptr<net::IOBufferWithSize> buffer,
             const base::Closure& done_task);

  
  int GetBufferSize();

  
  // to be written. Can be called on any thread.
  int GetBufferChunks();

  
  
  void Close();

 protected:
  struct PendingPacket;
  typedef std::list<PendingPacket*> DataQueue;

  DataQueue queue_;
  int buffer_size_;

  
  
  
  base::Closure PopQueue();

  

  // Returns next packet that needs to be written to the socket. Implementation
  
  virtual void GetNextPacket(net::IOBuffer** buffer, int* size) = 0;

  
  
  virtual base::Closure AdvanceBufferPosition(int written) = 0;

  
  virtual void OnError(int result) = 0;

 private:
  void DoWrite();
  void HandleWriteResult(int result, bool* write_again);
  void OnWritten(int result);

  
  void HandleError(int result);

  net::Socket* socket_;
  WriteFailedCallback write_failed_callback_;

  bool write_pending_;

  bool closed_;

  bool* destroyed_flag_;
};

class BufferedSocketWriter : public BufferedSocketWriterBase {
 public:
  BufferedSocketWriter();
  virtual ~BufferedSocketWriter();

 protected:
  virtual void GetNextPacket(net::IOBuffer** buffer, int* size) OVERRIDE;
  virtual base::Closure AdvanceBufferPosition(int written) OVERRIDE;
  virtual void OnError(int result) OVERRIDE;

 private:
  scoped_refptr<net::DrainableIOBuffer> current_buf_;
};

class BufferedDatagramWriter : public BufferedSocketWriterBase {
 public:
  BufferedDatagramWriter();
  virtual ~BufferedDatagramWriter();

 protected:
  virtual void GetNextPacket(net::IOBuffer** buffer, int* size) OVERRIDE;
  virtual base::Closure AdvanceBufferPosition(int written) OVERRIDE;
  virtual void OnError(int result) OVERRIDE;
};

}  
}  

#endif  
