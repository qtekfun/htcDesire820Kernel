// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_FILTERS_FFMPEG_GLUE_H_
#define MEDIA_FILTERS_FFMPEG_GLUE_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "media/base/media_export.h"

struct AVFormatContext;
struct AVIOContext;

namespace media {

class ScopedPtrAVFree;

class MEDIA_EXPORT FFmpegURLProtocol {
 public:
  
  
  virtual int Read(int size, uint8* data) = 0;

  
  
  virtual bool GetPosition(int64* position_out) = 0;

  
  virtual bool SetPosition(int64 position) = 0;

  
  
  virtual bool GetSize(int64* size_out) = 0;

  
  virtual bool IsStreaming() = 0;
};

class MEDIA_EXPORT FFmpegGlue {
 public:
  static void InitializeFFmpeg();

  
  explicit FFmpegGlue(FFmpegURLProtocol* protocol);
  ~FFmpegGlue();

  
  
  bool OpenContext();
  AVFormatContext* format_context() { return format_context_; }

 private:
  bool open_called_;
  AVFormatContext* format_context_;
  scoped_ptr_malloc<AVIOContext, ScopedPtrAVFree> avio_context_;

  DISALLOW_COPY_AND_ASSIGN(FFmpegGlue);
};

}  

#endif  
