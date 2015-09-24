// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SERIAL_SERIAL_IO_HANDLER_H_
#define CHROME_BROWSER_EXTENSIONS_API_SERIAL_SERIAL_IO_HANDLER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/platform_file.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/common/extensions/api/serial.h"
#include "net/base/io_buffer.h"

namespace extensions {

class SerialIoHandler : public base::NonThreadSafe,
                        public base::RefCounted<SerialIoHandler> {
 public:
  
  static scoped_refptr<SerialIoHandler> Create();

  
  
  typedef base::Callback<void(const std::string& data,
                              api::serial::ReceiveError error)>
      ReadCompleteCallback;

  // Called with the number of bytes written and a result code. Note that an
  // error result does not necessarily imply 0 bytes written.
  typedef base::Callback<void(int bytes_written, api::serial::SendError error)>
      WriteCompleteCallback;

  
  
  void Initialize(base::PlatformFile file,
                  const ReadCompleteCallback& read_callback,
                  const WriteCompleteCallback& write_callback);

  
  
  
  void Read(int max_bytes);

  
  
  
  void Write(const std::string& data);

  
  bool IsReadPending() const;

  
  bool IsWritePending() const;

  
  void CancelRead(api::serial::ReceiveError reason);

  
  void CancelWrite(api::serial::SendError reason);

 protected:
  SerialIoHandler();
  virtual ~SerialIoHandler();

  
  
  virtual void InitializeImpl() {}

  
  
  
  
  
  virtual void ReadImpl() = 0;

  
  
  
  
  
  virtual void WriteImpl() = 0;

  
  virtual void CancelReadImpl() = 0;

  
  virtual void CancelWriteImpl() = 0;

  
  
  
  void ReadCompleted(int bytes_read, api::serial::ReceiveError error);

  
  
  
  void WriteCompleted(int bytes_written, api::serial::SendError error);

  
  
  
  void QueueReadCompleted(int bytes_read, api::serial::ReceiveError error);

  
  
  
  void QueueWriteCompleted(int bytes_written, api::serial::SendError error);

  base::PlatformFile file() const {
    return file_;
  }

  net::IOBuffer* pending_read_buffer() const {
    return pending_read_buffer_.get();
  }

  int pending_read_buffer_len() const {
    return pending_read_buffer_len_;
  }

  api::serial::ReceiveError read_cancel_reason() const {
    return read_cancel_reason_;
  }

  bool read_canceled() const {
    return read_canceled_;
  }

  net::IOBuffer* pending_write_buffer() const {
    return pending_write_buffer_.get();
  }

  int pending_write_buffer_len() const {
    return pending_write_buffer_len_;
  }

  api::serial::SendError write_cancel_reason() const {
    return write_cancel_reason_;
  }

  bool write_canceled() const {
    return write_canceled_;
  }

 private:
  friend class base::RefCounted<SerialIoHandler>;

  base::PlatformFile file_;

  scoped_refptr<net::IOBuffer> pending_read_buffer_;
  int pending_read_buffer_len_;
  api::serial::ReceiveError read_cancel_reason_;
  bool read_canceled_;

  scoped_refptr<net::IOBuffer> pending_write_buffer_;
  int pending_write_buffer_len_;
  api::serial::SendError write_cancel_reason_;
  bool write_canceled_;

  ReadCompleteCallback read_complete_;
  WriteCompleteCallback write_complete_;

  DISALLOW_COPY_AND_ASSIGN(SerialIoHandler);
};

}  

#endif  
