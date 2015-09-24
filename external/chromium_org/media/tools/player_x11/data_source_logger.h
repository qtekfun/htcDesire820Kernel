// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_TOOLS_PLAYER_X11_DATA_SOURCE_LOGGER_H_
#define MEDIA_TOOLS_PLAYER_X11_DATA_SOURCE_LOGGER_H_

#include "media/base/data_source.h"

class DataSourceLogger : public media::DataSource {
 public:
  
  
  
  
  
  
  
  DataSourceLogger(scoped_ptr<DataSource> data_source,
                   bool force_streaming);
  virtual ~DataSourceLogger();

  
  virtual void set_host(media::DataSourceHost* host) OVERRIDE;
  virtual void Stop(const base::Closure& closure) OVERRIDE;
  virtual void Read(
      int64 position, int size, uint8* data,
      const media::DataSource::ReadCB& read_cb) OVERRIDE;
  virtual bool GetSize(int64* size_out) OVERRIDE;
  virtual bool IsStreaming() OVERRIDE;
  virtual void SetBitrate(int bitrate) OVERRIDE;

 private:
  scoped_ptr<media::DataSource> data_source_;
  bool streaming_;

  DISALLOW_COPY_AND_ASSIGN(DataSourceLogger);
};

#endif  
