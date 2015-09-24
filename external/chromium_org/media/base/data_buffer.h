// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_DATA_BUFFER_H_
#define MEDIA_BASE_DATA_BUFFER_H_

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT DataBuffer : public base::RefCountedThreadSafe<DataBuffer> {
 public:
  
  explicit DataBuffer(int buffer_size);

  
  DataBuffer(scoped_ptr<uint8[]> buffer, int buffer_size);

  
  
  
  static scoped_refptr<DataBuffer> CopyFrom(const uint8* data, int size);

  
  
  
  
  static scoped_refptr<DataBuffer> CreateEOSBuffer();

  base::TimeDelta timestamp() const {
    DCHECK(!end_of_stream());
    return timestamp_;
  }

  void set_timestamp(const base::TimeDelta& timestamp) {
    DCHECK(!end_of_stream());
    timestamp_ = timestamp;
  }

  base::TimeDelta duration() const {
    DCHECK(!end_of_stream());
    return duration_;
  }

  void set_duration(const base::TimeDelta& duration) {
    DCHECK(!end_of_stream());
    duration_ = duration;
  }

  const uint8* data() const {
    DCHECK(!end_of_stream());
    return data_.get();
  }

  uint8* writable_data() {
    DCHECK(!end_of_stream());
    return data_.get();
  }

  
  
  
  int data_size() const {
    DCHECK(!end_of_stream());
    return data_size_;
  }

  void set_data_size(int data_size) {
    DCHECK(!end_of_stream());
    CHECK_LE(data_size, buffer_size_);
    data_size_ = data_size;
  }

  
  bool end_of_stream() const { return data_ == NULL; }

 protected:
  friend class base::RefCountedThreadSafe<DataBuffer>;

  
  
  
  
  DataBuffer(const uint8* data, int data_size);

  virtual ~DataBuffer();

 private:
  base::TimeDelta timestamp_;
  base::TimeDelta duration_;

  scoped_ptr<uint8[]> data_;
  int buffer_size_;
  int data_size_;

  DISALLOW_COPY_AND_ASSIGN(DataBuffer);
};

}  

#endif  
