// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SPEECH_CHUNKED_BYTE_BUFFER_H_
#define CONTENT_BROWSER_SPEECH_CHUNKED_BYTE_BUFFER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT ChunkedByteBuffer {
 public:
  ChunkedByteBuffer();
  ~ChunkedByteBuffer();

  
  void Append(const uint8* start, size_t length);

  
  void Append(const std::string& string);

  
  bool HasChunks() const;

  
  
  scoped_ptr< std::vector<uint8> > PopChunk();

  
  void Clear();

  
  size_t GetTotalLength() const { return total_bytes_stored_; }

 private:
  struct Chunk {
    Chunk();
    ~Chunk();

    std::vector<uint8> header;
    scoped_ptr< std::vector<uint8> > content;
    size_t ExpectedContentLength() const;

   private:
    DISALLOW_COPY_AND_ASSIGN(Chunk);
  };

  ScopedVector<Chunk> chunks_;
  scoped_ptr<Chunk> partial_chunk_;
  size_t total_bytes_stored_;

  DISALLOW_COPY_AND_ASSIGN(ChunkedByteBuffer);
};


}  

#endif  
