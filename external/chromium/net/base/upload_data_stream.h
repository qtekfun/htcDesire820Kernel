// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_UPLOAD_DATA_STREAM_H_
#define NET_BASE_UPLOAD_DATA_STREAM_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "net/base/upload_data.h"

#ifdef ANDROID
#include "android/jni/platform_file_jni.h"
#endif

namespace net {

class FileStream;
class IOBuffer;

class UploadDataStream {
 public:
  ~UploadDataStream();

  
  
  
  static UploadDataStream* Create(UploadData* data, int* error_code);

  
  IOBuffer* buf() const { return buf_; }
  size_t buf_len() const { return buf_len_; }

  
  
  
  
  size_t GetMaxBufferSize() const { return kBufSize; }

  
  
  
  void MarkConsumedAndFillBuffer(size_t num_bytes);

  
  void set_chunk_callback(ChunkCallback* callback) {
    data_->set_chunk_callback(callback);
  }

  
  
  
  
  uint64 size() const { return total_size_; }
  uint64 position() const { return current_position_; }

  bool is_chunked() const { return data_->is_chunked(); }

  
  
  bool eof() const { return eof_; }

  
  
  
  
  bool IsOnLastChunk() const;

#if defined(UNIT_TEST)
  static void set_merge_chunks(bool merge) { merge_chunks_ = merge; }
#endif

 private:
  enum { kBufSize = 16384 };

  
  
  explicit UploadDataStream(UploadData* data);

  
  
  
  int FillBuf();

  scoped_refptr<UploadData> data_;

  
  
  
  
  scoped_refptr<IOBuffer> buf_;
  size_t buf_len_;

  // Index of the upload element to be written to the send buffer next.
  size_t next_element_;

  
  
  size_t next_element_offset_;

  
  
  scoped_ptr<FileStream> next_element_stream_;

#ifdef ANDROID
  scoped_ptr<android::JavaISWrapper> next_element_java_stream_;
#endif

  
  
  uint64 next_element_remaining_;

  
  uint64 total_size_;
  uint64 current_position_;

  
  bool eof_;

  
  
  static bool merge_chunks_;

  DISALLOW_COPY_AND_ASSIGN(UploadDataStream);
};

}  

#endif  
