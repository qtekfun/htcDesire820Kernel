// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "media/base/video_frame.h"

namespace media {
namespace cast {

double I420PSNR(const scoped_refptr<media::VideoFrame>& frame1,
                const scoped_refptr<media::VideoFrame>& frame2);

void PopulateVideoFrame(VideoFrame* frame, int start_value);

bool PopulateVideoFrameFromFile(VideoFrame* frame, FILE* video_file);

}  
}  
