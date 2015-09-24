// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_BYTE_STREAM_H_
#define CONTENT_BROWSER_BYTE_STREAM_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "net/base/io_buffer.h"

namespace base {
class SequencedTaskRunner;
}

namespace content {

// and the sink retrieves bytes already written via |ByteStreamReader::Read|.
class CONTENT_EXPORT ByteStreamWriter {
 public:
  
  
  static const int kFractionBufferBeforeSending;

  virtual ~ByteStreamWriter() = 0;

  
  
  
  virtual bool Write(scoped_refptr<net::IOBuffer> buffer,
                     size_t byte_count) = 0;

  
  
  
  
  virtual void Flush() = 0;

  
  
  virtual void Close(int status) = 0;

  
  
  
  
  
  
  
  
  
  
  virtual void RegisterCallback(const base::Closure& source_callback) = 0;

  
  
  virtual size_t GetTotalBufferedBytes() const = 0;
};

class CONTENT_EXPORT ByteStreamReader {
 public:
  
  
  static const int kFractionReadBeforeWindowUpdate;

  enum StreamState { STREAM_EMPTY, STREAM_HAS_DATA, STREAM_COMPLETE };

  virtual ~ByteStreamReader() = 0;

  
  
  
  
  
  
  virtual StreamState Read(scoped_refptr<net::IOBuffer>* data,
                           size_t* length) = 0;

  
  virtual int GetStatus() const = 0;

  
  
  
  
  
  
  
  virtual void RegisterCallback(const base::Closure& sink_callback) = 0;
};

CONTENT_EXPORT void CreateByteStream(
    scoped_refptr<base::SequencedTaskRunner> input_task_runner,
    scoped_refptr<base::SequencedTaskRunner> output_task_runner,
    size_t buffer_size,
    scoped_ptr<ByteStreamWriter>* input,
    scoped_ptr<ByteStreamReader>* output);

}  

#endif  
