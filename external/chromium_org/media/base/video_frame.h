// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_VIDEO_FRAME_H_
#define MEDIA_BASE_VIDEO_FRAME_H_

#include "base/callback.h"
#include "base/md5.h"
#include "base/memory/shared_memory.h"
#include "gpu/command_buffer/common/mailbox.h"
#include "media/base/buffers.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

class SkBitmap;

namespace media {

class MEDIA_EXPORT VideoFrame : public base::RefCountedThreadSafe<VideoFrame> {
 public:
  enum {
    kFrameSizeAlignment = 16,
    kFrameSizePadding = 16,
    kFrameAddressAlignment = 32
  };

  enum {
    kMaxPlanes = 4,

    kYPlane = 0,
    kUPlane = 1,
    kVPlane = 2,
    kAPlane = 3,
  };

  
  
  
  
  enum Format {
    UNKNOWN = 0,  
    YV12 = 1,  
    YV16 = 2,  
    I420 = 3,  
    YV12A = 4,  
#if defined(VIDEO_HOLE)
    HOLE = 5,  
#endif  
    NATIVE_TEXTURE = 6,  
    YV12J = 7,  
    HISTOGRAM_MAX,  
  };

  
  static std::string FormatToString(Format format);

  
  
  
  
  
  class MEDIA_EXPORT MailboxHolder {
   public:
    typedef base::Callback<void(uint32 sync_point)>
        TextureNoLongerNeededCallback;

    MailboxHolder(const gpu::Mailbox& mailbox,
                  unsigned sync_point,
                  const TextureNoLongerNeededCallback& release_callback);
    ~MailboxHolder();

    const gpu::Mailbox& mailbox() const { return mailbox_; }
    unsigned sync_point() const { return sync_point_; }

    void Resync(unsigned sync_point) { sync_point_ = sync_point; }

   private:

    gpu::Mailbox mailbox_;
    unsigned sync_point_;
    TextureNoLongerNeededCallback release_callback_;
  };


  
  
  
  
  
  
  
  static scoped_refptr<VideoFrame> CreateFrame(
      Format format,
      const gfx::Size& coded_size,
      const gfx::Rect& visible_rect,
      const gfx::Size& natural_size,
      base::TimeDelta timestamp);

  
  
  
  static bool IsValidConfig(Format format, const gfx::Size& coded_size,
                            const gfx::Rect& visible_rect,
                            const gfx::Size& natural_size);

  
  
  typedef base::Callback<void(const SkBitmap&)> ReadPixelsCB;

  
  
  
  
  
  
  

  
  
  static scoped_refptr<VideoFrame> WrapNativeTexture(
      scoped_ptr<MailboxHolder> mailbox_holder,
      uint32 texture_target,
      const gfx::Size& coded_size,
      const gfx::Rect& visible_rect,
      const gfx::Size& natural_size,
      base::TimeDelta timestamp,
      const ReadPixelsCB& read_pixels_cb,
      const base::Closure& no_longer_needed_cb);

  
  
  
  void ReadPixelsFromNativeTexture(const SkBitmap& pixels);

  
  
  
  
  
  
  static scoped_refptr<VideoFrame> WrapExternalPackedMemory(
      Format format,
      const gfx::Size& coded_size,
      const gfx::Rect& visible_rect,
      const gfx::Size& natural_size,
      uint8* data,
      size_t data_size,
      base::SharedMemoryHandle handle,
      base::TimeDelta timestamp,
      const base::Closure& no_longer_needed_cb);

  
  
  
  
  
  static scoped_refptr<VideoFrame> WrapExternalYuvData(
      Format format,
      const gfx::Size& coded_size,
      const gfx::Rect& visible_rect,
      const gfx::Size& natural_size,
      int32 y_stride,
      int32 u_stride,
      int32 v_stride,
      uint8* y_data,
      uint8* u_data,
      uint8* v_data,
      base::TimeDelta timestamp,
      const base::Closure& no_longer_needed_cb);

  
  
  static scoped_refptr<VideoFrame> WrapVideoFrame(
      const scoped_refptr<VideoFrame>& frame,
      const base::Closure& no_longer_needed_cb);

  
  static scoped_refptr<VideoFrame> CreateEOSFrame();

  
  static scoped_refptr<VideoFrame> CreateColorFrame(
      const gfx::Size& size,
      uint8 y, uint8 u, uint8 v,
      base::TimeDelta timestamp);

  
  
  static scoped_refptr<VideoFrame> CreateBlackFrame(const gfx::Size& size);

#if defined(VIDEO_HOLE)
  
  static scoped_refptr<VideoFrame> CreateHoleFrame(const gfx::Size& size);
#endif  

  static size_t NumPlanes(Format format);

  
  
  static size_t AllocationSize(Format format, const gfx::Size& coded_size);

  
  
  static size_t PlaneAllocationSize(Format format,
                                    size_t plane,
                                    const gfx::Size& coded_size);

  Format format() const { return format_; }

  const gfx::Size& coded_size() const { return coded_size_; }
  const gfx::Rect& visible_rect() const { return visible_rect_; }
  const gfx::Size& natural_size() const { return natural_size_; }

  int stride(size_t plane) const;

  
  
  
  
  int row_bytes(size_t plane) const;
  int rows(size_t plane) const;

  
  
  uint8* data(size_t plane) const;

  
  
  
  MailboxHolder* texture_mailbox() const;

  
  uint32 texture_target() const;

  
  base::SharedMemoryHandle shared_memory_handle() const;

  
  bool end_of_stream() const { return end_of_stream_; }

  base::TimeDelta GetTimestamp() const {
    return timestamp_;
  }
  void SetTimestamp(const base::TimeDelta& timestamp) {
    timestamp_ = timestamp;
  }

  
  
  void HashFrameForTesting(base::MD5Context* context);

 private:
  friend class base::RefCountedThreadSafe<VideoFrame>;
  
  VideoFrame(Format format,
             const gfx::Size& coded_size,
             const gfx::Rect& visible_rect,
             const gfx::Size& natural_size,
             base::TimeDelta timestamp,
             bool end_of_stream);
  virtual ~VideoFrame();

  void AllocateYUV();

  
  bool IsValidPlane(size_t plane) const;

  
  Format format_;

  
  gfx::Size coded_size_;

  
  gfx::Rect visible_rect_;

  
  
  gfx::Size natural_size_;

  
  
  
  int32 strides_[kMaxPlanes];

  
  uint8* data_[kMaxPlanes];

  
  scoped_ptr<MailboxHolder> texture_mailbox_holder_;
  uint32 texture_target_;
  ReadPixelsCB read_pixels_cb_;

  
  base::SharedMemoryHandle shared_memory_handle_;

  base::Closure no_longer_needed_cb_;

  base::TimeDelta timestamp_;

  const bool end_of_stream_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(VideoFrame);
};

}  

#endif  
