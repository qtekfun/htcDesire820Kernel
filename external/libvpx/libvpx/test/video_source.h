/*
 *  Copyright (c) 2012 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#ifndef TEST_VIDEO_SOURCE_H_
#define TEST_VIDEO_SOURCE_H_

#include <cstdio>
#include <cstdlib>
#include <string>
#include "test/acm_random.h"
#include "vpx/vpx_encoder.h"

namespace libvpx_test {

#define TO_STRING(S) #S
#define STRINGIFY(S) TO_STRING(S)

static std::string GetDataPath() {
  const char *const data_path = getenv("LIBVPX_TEST_DATA_PATH");
  if (data_path == NULL) {
#ifdef LIBVPX_TEST_DATA_PATH
    
    
    
    return STRINGIFY(LIBVPX_TEST_DATA_PATH);
#else
    return ".";
#endif
  }
  return data_path;
}

#undef TO_STRING
#undef STRINGIFY

static FILE *OpenTestDataFile(const std::string& file_name) {
  const std::string path_to_source = GetDataPath() + "/" + file_name;
  return fopen(path_to_source.c_str(), "rb");
}

class VideoSource {
 public:
  virtual ~VideoSource() {}

  
  virtual void Begin() = 0;

  
  virtual void Next() = 0;

  
  virtual vpx_image_t *img() const = 0;

  
  virtual vpx_codec_pts_t pts() const = 0;

  
  virtual unsigned long duration() const = 0;

  
  virtual vpx_rational_t timebase() const = 0;

  
  virtual unsigned int frame() const = 0;

  
  virtual unsigned int limit() const = 0;
};


class DummyVideoSource : public VideoSource {
 public:
  DummyVideoSource() : img_(NULL), limit_(100), width_(0), height_(0) {
    SetSize(80, 64);
  }

  virtual ~DummyVideoSource() { vpx_img_free(img_); }

  virtual void Begin() {
    frame_ = 0;
    FillFrame();
  }

  virtual void Next() {
    ++frame_;
    FillFrame();
  }

  virtual vpx_image_t *img() const {
    return (frame_ < limit_) ? img_ : NULL;
  }

  
  virtual vpx_codec_pts_t pts() const { return frame_; }

  virtual unsigned long duration() const { return 1; }

  virtual vpx_rational_t timebase() const {
    const vpx_rational_t t = {1, 30};
    return t;
  }

  virtual unsigned int frame() const { return frame_; }

  virtual unsigned int limit() const { return limit_; }

  void SetSize(unsigned int width, unsigned int height) {
    if (width != width_ || height != height_) {
      vpx_img_free(img_);
      raw_sz_ = ((width + 31)&~31) * height * 3 / 2;
      img_ = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 32);
      width_ = width;
      height_ = height;
    }
  }

 protected:
  virtual void FillFrame() { memset(img_->img_data, 0, raw_sz_); }

  vpx_image_t *img_;
  size_t       raw_sz_;
  unsigned int limit_;
  unsigned int frame_;
  unsigned int width_;
  unsigned int height_;
};


class RandomVideoSource : public DummyVideoSource {
 public:
  RandomVideoSource(int seed = ACMRandom::DeterministicSeed())
      : rnd_(seed),
        seed_(seed) { }

 protected:
  
  virtual void Begin() {
    frame_ = 0;
    rnd_.Reset(seed_);
    FillFrame();
  }

  
  
  virtual void FillFrame() {
    if (frame_ % 30 < 15)
      for (size_t i = 0; i < raw_sz_; ++i)
        img_->img_data[i] = rnd_.Rand8();
    else
      memset(img_->img_data, 0, raw_sz_);
  }

  ACMRandom rnd_;
  int seed_;
};

class CompressedVideoSource {
 public:
  virtual ~CompressedVideoSource() {}

  virtual void Init() = 0;

  
  virtual void Begin() = 0;

  
  virtual void Next() = 0;

  virtual const uint8_t *cxdata() const = 0;

  virtual const unsigned int frame_size() const = 0;

  virtual const unsigned int frame_number() const = 0;
};

}  

#endif  
