// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_UPLOAD_DATA_STREAM_H_
#define NET_BASE_UPLOAD_DATA_STREAM_H_

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"

namespace net {

class DrainableIOBuffer;
class IOBuffer;
class UploadElementReader;

class NET_EXPORT UploadDataStream {
 public:
  
  enum Chunked { CHUNKED };

  
  UploadDataStream(ScopedVector<UploadElementReader> element_readers,
                   int64 identifier);

  
  UploadDataStream(Chunked chunked, int64 identifier);

  ~UploadDataStream();

  
  static UploadDataStream* CreateWithReader(
      scoped_ptr<UploadElementReader> reader,
      int64 identifier);

  
  
  
  
  
  
  
  
  
  
  
  
  int Init(const CompletionCallback& callback);

  
  
  
  
  
  
  
  
  
  
  int Read(IOBuffer* buf, int buf_len, const CompletionCallback& callback);

  
  
  
  int64 identifier() const { return identifier_; }

  
  
  
  
  
  uint64 size() const { return total_size_; }
  uint64 position() const { return current_position_; }

  bool is_chunked() const { return is_chunked_; }
  bool last_chunk_appended() const { return last_chunk_appended_; }

  const ScopedVector<UploadElementReader>& element_readers() const {
    return element_readers_;
  }

  
  
  bool IsEOF() const;

  
  bool IsInMemory() const;

  
  void AppendChunk(const char* bytes, int bytes_len, bool is_last_chunk);

  
  void Reset();

 private:
  
  
  int InitInternal(int start_index, const CompletionCallback& callback);

  
  void ResumePendingInit(int start_index,
                         const CompletionCallback& callback,
                         int previous_result);

  
  
  int ReadInternal(scoped_refptr<DrainableIOBuffer> buf,
                   const CompletionCallback& callback);

  
  void ResumePendingRead(scoped_refptr<DrainableIOBuffer> buf,
                         const CompletionCallback& callback,
                         int previous_result);

  
  
  void ProcessReadResult(scoped_refptr<DrainableIOBuffer> buf,
                         int result);

  ScopedVector<UploadElementReader> element_readers_;

  
  
  
  size_t element_index_;

  
  
  uint64 total_size_;
  uint64 current_position_;

  const int64 identifier_;

  const bool is_chunked_;
  bool last_chunk_appended_;

  
  bool read_failed_;

  
  bool initialized_successfully_;

  
  base::Closure pending_chunked_read_callback_;

  base::WeakPtrFactory<UploadDataStream> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(UploadDataStream);
};

}  

#endif  
