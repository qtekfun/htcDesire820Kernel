// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_DATA_SOURCE_H_
#define MEDIA_BASE_DATA_SOURCE_H_

#include "base/callback.h"
#include "base/time/time.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT DataSourceHost {
 public:
  
  virtual void SetTotalBytes(int64 total_bytes) = 0;

  
  
  
  virtual void AddBufferedByteRange(int64 start, int64 end) = 0;

  
  virtual void AddBufferedTimeRange(base::TimeDelta start,
                                    base::TimeDelta end) = 0;

 protected:
  virtual ~DataSourceHost();
};

class MEDIA_EXPORT DataSource {
 public:
  typedef base::Callback<void(int64, int64)> StatusCallback;
  typedef base::Callback<void(int)> ReadCB;
  static const int kReadError;

  DataSource();
  virtual ~DataSource();

  virtual void set_host(DataSourceHost* host);

  
  
  
  virtual void Read(int64 position, int size, uint8* data,
                    const DataSource::ReadCB& read_cb) = 0;

  
  
  virtual void Stop(const base::Closure& callback) = 0;

  
  
  virtual bool GetSize(int64* size_out) = 0;

  
  
  virtual bool IsStreaming() = 0;

  
  
  virtual void SetBitrate(int bitrate) = 0;

 protected:
  DataSourceHost* host();

 private:
  DataSourceHost* host_;

  DISALLOW_COPY_AND_ASSIGN(DataSource);
};

}  

#endif  
