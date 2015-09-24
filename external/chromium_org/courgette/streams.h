// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COURGETTE_STREAMS_H_
#define COURGETTE_STREAMS_H_

#include <stdio.h>  
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"

#include "courgette/memory_allocator.h"
#include "courgette/region.h"


namespace courgette {

class SourceStream;
class SinkStream;

static const unsigned int kMaxStreams = 10;

class SourceStream {
 public:
  SourceStream() : start_(NULL), end_(NULL), current_(NULL) {}

  
  
  
  void Init(const void* pointer, size_t length) {
    start_ = static_cast<const uint8*>(pointer);
    end_ = start_ + length;
    current_ = start_;
  }

  
  
  
  void Init(const Region& region) { Init(region.start(), region.length()); }

  
  
  
  void Init(const std::string& string) { Init(string.c_str(), string.size()); }

  // Initializes the SourceStream to yield the bytes written to |sink|. |sink|
  
  // written to after |this| is initialized.
  void Init(const SinkStream& sink);

  
  size_t Remaining() const { return end_ - current_; }

  
  size_t OriginalLength() const { return end_ - start_; }

  const uint8* Buffer() const { return current_; }
  bool Empty() const { return current_ == end_; }

  
  
  bool Read(void* destination, size_t byte_count);

  
  
  bool ReadVarint32(uint32* output_value);

  
  
  bool ReadVarint32Signed(int32* output_value);

  
  
  
  bool ShareSubstream(size_t offset, size_t length, SourceStream* substream);

  
  
  
  bool ShareSubstream(size_t length, SourceStream* substream) {
    return ShareSubstream(0, length, substream);
  }

  
  
  
  bool ReadSubstream(size_t length, SourceStream* substream);

  
  bool Skip(size_t byte_count);

 private:
  const uint8* start_;     
  const uint8* end_;       
  const uint8* current_;   

  DISALLOW_COPY_AND_ASSIGN(SourceStream);
};

class SinkStream {
 public:
  SinkStream() {}
  ~SinkStream() {}

  
  CheckBool Write(const void* data, size_t byte_count) WARN_UNUSED_RESULT;

  
  CheckBool WriteVarint32(uint32 value) WARN_UNUSED_RESULT;

  
  CheckBool WriteVarint32Signed(int32 value) WARN_UNUSED_RESULT;

  
  
  CheckBool WriteSizeVarint32(size_t value) WARN_UNUSED_RESULT;

  
  
  CheckBool Append(SinkStream* other) WARN_UNUSED_RESULT;

  
  size_t Length() const { return buffer_.size(); }

  
  
  
  const uint8* Buffer() const {
    return reinterpret_cast<const uint8*>(buffer_.data());
  }

  
  
  CheckBool Reserve(size_t length) WARN_UNUSED_RESULT {
    return buffer_.reserve(length + buffer_.size());
  }

  
  void Retire();

 private:
  NoThrowBuffer<char> buffer_;

  DISALLOW_COPY_AND_ASSIGN(SinkStream);
};

class SourceStreamSet {
 public:
  SourceStreamSet();
  ~SourceStreamSet();

  
  
  
  
  // The layout of the streams are as written by SinkStreamSet::CopyTo.
  
  bool Init(const void* source, size_t byte_count);

  
  
  bool Init(SourceStream* source);

  
  SourceStream* stream(size_t id) { return id < count_ ? &streams_[id] : NULL; }

  
  bool ReadSet(SourceStreamSet* set);

  
  bool Empty() const;

 private:
  size_t count_;
  SourceStream streams_[kMaxStreams];

  DISALLOW_COPY_AND_ASSIGN(SourceStreamSet);
};

class SinkStreamSet {
 public:
  SinkStreamSet();
  ~SinkStreamSet();

  
  
  void Init(size_t stream_index_limit);

  
  SinkStream* stream(size_t id) { return id < count_ ? &streams_[id] : NULL; }

  
  
  
  CheckBool CopyTo(SinkStream* combined_stream) WARN_UNUSED_RESULT;

  
  // Stream zero first has some metadata written to it.  |set| becomes retired.
  
  CheckBool WriteSet(SinkStreamSet* set) WARN_UNUSED_RESULT;

 private:
  CheckBool CopyHeaderTo(SinkStream* stream) WARN_UNUSED_RESULT;

  size_t count_;
  SinkStream streams_[kMaxStreams];

  DISALLOW_COPY_AND_ASSIGN(SinkStreamSet);
};

}  
#endif  
