// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_BASE_COMPOUND_BUFFER_H_
#define REMOTING_BASE_COMPOUND_BUFFER_H_

#include <deque>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "google/protobuf/io/zero_copy_stream.h"

namespace net {
class IOBuffer;
class IOBufferWithSize;
}  

namespace remoting {

class CompoundBuffer {
 public:
  CompoundBuffer();
  ~CompoundBuffer();

  void Clear();

  
  
  
  void Append(net::IOBuffer* buffer, int size);
  void Append(net::IOBuffer* buffer, const char* start, int size);
  void Append(const CompoundBuffer& buffer);
  void Prepend(net::IOBuffer* buffer, int size);
  void Prepend(net::IOBuffer* buffer, const char* start, int size);
  void Prepend(const CompoundBuffer& buffer);

  
  void AppendCopyOf(const char* data, int data_size);
  void PrependCopyOf(const char* data, int data_size);

  
  void CropFront(int bytes);
  void CropBack(int bytes);

  
  int total_bytes() const { return total_bytes_; }

  
  
  
  void Lock();

  
  bool locked() const { return locked_; }

  
  
  net::IOBufferWithSize* ToIOBufferWithSize() const;

  
  void CopyTo(char* data, int data_size) const;

  
  
  void CopyFrom(const CompoundBuffer& source, int start, int end);

 private:
  friend class CompoundBufferInputStream;

  struct DataChunk {
    DataChunk(net::IOBuffer* buffer, const char* start, int size);
    ~DataChunk();

    scoped_refptr<net::IOBuffer> buffer;
    const char* start;
    int size;
  };
  typedef std::deque<DataChunk> DataChunkList;

  DataChunkList chunks_;
  int total_bytes_;
  bool locked_;

  DISALLOW_COPY_AND_ASSIGN(CompoundBuffer);
};

class CompoundBufferInputStream
    : public google::protobuf::io::ZeroCopyInputStream  {
 public:
  
  explicit CompoundBufferInputStream(const CompoundBuffer* buffer);
  virtual ~CompoundBufferInputStream();

  int position() const { return position_; }

  
  virtual bool Next(const void** data, int* size) OVERRIDE;
  virtual void BackUp(int count) OVERRIDE;
  virtual bool Skip(int count) OVERRIDE;
  virtual int64 ByteCount() const OVERRIDE;

 private:
  const CompoundBuffer* buffer_;

  size_t current_chunk_;
  int current_chunk_position_;
  int position_;
  int last_returned_size_;
};

}  

#endif  
