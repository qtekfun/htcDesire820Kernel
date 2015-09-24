// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_VIDEO_FRAME_POOL_H_
#define MEDIA_BASE_VIDEO_FRAME_POOL_H_

#include "media/base/media_export.h"
#include "media/base/video_frame.h"

namespace media {

class MEDIA_EXPORT VideoFramePool {
 public:
  VideoFramePool();
  ~VideoFramePool();

  
  
  
  scoped_refptr<VideoFrame> CreateFrame(VideoFrame::Format format,
                                        const gfx::Size& coded_size,
                                        const gfx::Rect& visible_rect,
                                        const gfx::Size& natural_size,
                                        base::TimeDelta timestamp);

protected:
  friend class VideoFramePoolTest;

  
  size_t GetPoolSizeForTesting() const;

 private:
  class PoolImpl;
  scoped_refptr<PoolImpl> pool_;

  DISALLOW_COPY_AND_ASSIGN(VideoFramePool);
};

}  

#endif  
