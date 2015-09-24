// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CDM_PPAPI_CDM_HELPERS_H_
#define MEDIA_CDM_PPAPI_CDM_HELPERS_H_

#include <map>
#include <utility>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "build/build_config.h"
#include "media/cdm/ppapi/api/content_decryption_module.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/cpp/dev/buffer_dev.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/logging.h"

namespace media {

class PpbBuffer : public cdm::Buffer {
 public:
  static PpbBuffer* Create(const pp::Buffer_Dev& buffer, uint32_t buffer_id) {
    PP_DCHECK(buffer.data());
    PP_DCHECK(buffer.size());
    PP_DCHECK(buffer_id);
    return new PpbBuffer(buffer, buffer_id);
  }

  
  virtual void Destroy() OVERRIDE { delete this; }

  virtual uint32_t Capacity() const OVERRIDE { return buffer_.size(); }

  virtual uint8_t* Data() OVERRIDE {
    return static_cast<uint8_t*>(buffer_.data());
  }

  virtual void SetSize(uint32_t size) OVERRIDE {
    PP_DCHECK(size <= Capacity());
    if (size > Capacity()) {
      size_ = 0;
      return;
    }

    size_ = size;
  }

  virtual uint32_t Size() const OVERRIDE { return size_; }

  pp::Buffer_Dev buffer_dev() const { return buffer_; }

  uint32_t buffer_id() const { return buffer_id_; }

 private:
  PpbBuffer(pp::Buffer_Dev buffer, uint32_t buffer_id)
      : buffer_(buffer),
        buffer_id_(buffer_id),
        size_(0) {}
  virtual ~PpbBuffer() {}

  pp::Buffer_Dev buffer_;
  uint32_t buffer_id_;
  uint32_t size_;

  DISALLOW_COPY_AND_ASSIGN(PpbBuffer);
};

class PpbBufferAllocator {
 public:
  explicit PpbBufferAllocator(pp::Instance* instance)
      : instance_(instance),
        next_buffer_id_(1) {}
  ~PpbBufferAllocator() {}

  cdm::Buffer* Allocate(uint32_t capacity);

  
  
  void Release(uint32_t buffer_id);

 private:
  typedef std::map<uint32_t, pp::Buffer_Dev> AllocatedBufferMap;
  typedef std::multimap<uint32_t, std::pair<uint32_t, pp::Buffer_Dev> >
      FreeBufferMap;

  pp::Buffer_Dev AllocateNewBuffer(uint32_t capacity);

  pp::Instance* const instance_;
  uint32_t next_buffer_id_;
  AllocatedBufferMap allocated_buffers_;
  FreeBufferMap free_buffers_;

  DISALLOW_COPY_AND_ASSIGN(PpbBufferAllocator);
};

class DecryptedBlockImpl : public cdm::DecryptedBlock {
 public:
  DecryptedBlockImpl() : buffer_(NULL), timestamp_(0) {}
  virtual ~DecryptedBlockImpl() { if (buffer_) buffer_->Destroy(); }

  virtual void SetDecryptedBuffer(cdm::Buffer* buffer) OVERRIDE {
    buffer_ = static_cast<PpbBuffer*>(buffer);
  }
  virtual cdm::Buffer* DecryptedBuffer() OVERRIDE { return buffer_; }

  virtual void SetTimestamp(int64_t timestamp) OVERRIDE {
    timestamp_ = timestamp;
  }
  virtual int64_t Timestamp() const OVERRIDE { return timestamp_; }

 private:
  PpbBuffer* buffer_;
  int64_t timestamp_;

  DISALLOW_COPY_AND_ASSIGN(DecryptedBlockImpl);
};

class VideoFrameImpl : public cdm::VideoFrame {
 public:
  VideoFrameImpl();
  virtual ~VideoFrameImpl();

  virtual void SetFormat(cdm::VideoFormat format) OVERRIDE {
    format_ = format;
  }
  virtual cdm::VideoFormat Format() const OVERRIDE { return format_; }

  virtual void SetSize(cdm::Size size) OVERRIDE { size_ = size; }
  virtual cdm::Size Size() const OVERRIDE { return size_; }

  virtual void SetFrameBuffer(cdm::Buffer* frame_buffer) OVERRIDE {
    frame_buffer_ = static_cast<PpbBuffer*>(frame_buffer);
  }
  virtual cdm::Buffer* FrameBuffer() OVERRIDE { return frame_buffer_; }

  virtual void SetPlaneOffset(cdm::VideoFrame::VideoPlane plane,
                              uint32_t offset) OVERRIDE {
    PP_DCHECK(plane < kMaxPlanes);
    plane_offsets_[plane] = offset;
  }
  virtual uint32_t PlaneOffset(VideoPlane plane) OVERRIDE {
    PP_DCHECK(plane < kMaxPlanes);
    return plane_offsets_[plane];
  }

  virtual void SetStride(VideoPlane plane, uint32_t stride) OVERRIDE {
    PP_DCHECK(plane < kMaxPlanes);
    strides_[plane] = stride;
  }
  virtual uint32_t Stride(VideoPlane plane) OVERRIDE {
    PP_DCHECK(plane < kMaxPlanes);
    return strides_[plane];
  }

  virtual void SetTimestamp(int64_t timestamp) OVERRIDE {
    timestamp_ = timestamp;
  }
  virtual int64_t Timestamp() const OVERRIDE { return timestamp_; }

 private:
  
  cdm::VideoFormat format_;

  
  cdm::Size size_;

  
  PpbBuffer* frame_buffer_;

  
  uint32_t plane_offsets_[kMaxPlanes];

  
  
  
  uint32_t strides_[kMaxPlanes];

  
  int64_t timestamp_;

  DISALLOW_COPY_AND_ASSIGN(VideoFrameImpl);
};

class AudioFramesImpl : public cdm::AudioFrames_1,
                        public cdm::AudioFrames_2 {
 public:
  AudioFramesImpl() : buffer_(NULL), format_(cdm::kUnknownAudioFormat) {}
  virtual ~AudioFramesImpl() {
    if (buffer_)
      buffer_->Destroy();
  }

  
  virtual void SetFrameBuffer(cdm::Buffer* buffer) OVERRIDE {
    buffer_ = static_cast<PpbBuffer*>(buffer);
  }
  virtual cdm::Buffer* FrameBuffer() OVERRIDE {
    return buffer_;
  }
  virtual void SetFormat(cdm::AudioFormat format) OVERRIDE {
    format_ = format;
  }
  virtual cdm::AudioFormat Format() const OVERRIDE {
    return format_;
  }

  cdm::Buffer* PassFrameBuffer() {
    PpbBuffer* temp_buffer = buffer_;
    buffer_ = NULL;
    return temp_buffer;
  }

 private:
  PpbBuffer* buffer_;
  cdm::AudioFormat format_;

  DISALLOW_COPY_AND_ASSIGN(AudioFramesImpl);
};

}  

#endif  
