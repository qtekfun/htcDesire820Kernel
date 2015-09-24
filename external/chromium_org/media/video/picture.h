// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_VIDEO_PICTURE_H_
#define MEDIA_VIDEO_PICTURE_H_

#include "base/basictypes.h"
#include "gpu/command_buffer/common/mailbox.h"
#include "media/base/media_export.h"
#include "ui/gfx/size.h"

namespace media {

class MEDIA_EXPORT PictureBuffer {
 public:
  PictureBuffer(int32 id, gfx::Size size, uint32 texture_id);
  PictureBuffer(int32 id,
                gfx::Size size,
                uint32 texture_id,
                const gpu::Mailbox& texture_mailbox);

  
  int32 id() const {
    return id_;
  }

  
  gfx::Size size() const {
    return size_;
  }

  
  
  
  uint32 texture_id() const {
    return texture_id_;
  }

  const gpu::Mailbox& texture_mailbox() const {
    return texture_mailbox_;
  }

 private:
  int32 id_;
  gfx::Size size_;
  uint32 texture_id_;
  gpu::Mailbox texture_mailbox_;
};

class MEDIA_EXPORT Picture {
 public:
  Picture(int32 picture_buffer_id, int32 bitstream_buffer_id);

  
  int32 picture_buffer_id() const {
    return picture_buffer_id_;
  }

  
  int32 bitstream_buffer_id() const {
    return bitstream_buffer_id_;
  }

  void set_bitstream_buffer_id(int32 bitstream_buffer_id) {
    bitstream_buffer_id_ = bitstream_buffer_id;
  }

 private:
  int32 picture_buffer_id_;
  int32 bitstream_buffer_id_;
};

}  

#endif  
