// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_BLOCKING_URL_PROTOCOL_H_
#define MEDIA_FILTERS_BLOCKING_URL_PROTOCOL_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/synchronization/waitable_event.h"
#include "media/filters/ffmpeg_glue.h"

namespace media {

class DataSource;

class MEDIA_EXPORT BlockingUrlProtocol : public FFmpegURLProtocol {
 public:
  
  
  
  
  
  BlockingUrlProtocol(DataSource* data_source, const base::Closure& error_cb);
  virtual ~BlockingUrlProtocol();

  
  
  void Abort();

  
  virtual int Read(int size, uint8* data) OVERRIDE;
  virtual bool GetPosition(int64* position_out) OVERRIDE;
  virtual bool SetPosition(int64 position) OVERRIDE;
  virtual bool GetSize(int64* size_out) OVERRIDE;
  virtual bool IsStreaming() OVERRIDE;

 private:
  
  
  void SignalReadCompleted(int size);

  DataSource* data_source_;
  base::Closure error_cb_;

  
  base::WaitableEvent aborted_;
  base::WaitableEvent read_complete_;

  
  int last_read_bytes_;

  
  int64 read_position_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(BlockingUrlProtocol);
};

}  

#endif  
