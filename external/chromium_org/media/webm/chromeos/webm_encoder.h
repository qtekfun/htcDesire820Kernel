// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_WEBM_CHROMEOS_WEBM_ENCODER_H_
#define MEDIA_WEBM_CHROMEOS_WEBM_ENCODER_H_

#include <stdio.h>
#include <stack>

#include "base/files/file_path.h"
#include "media/base/media_export.h"
#include "media/webm/chromeos/ebml_writer.h"

extern "C" {
#define VPX_CODEC_DISABLE_COMPAT 1
#include "third_party/libvpx/source/libvpx/vpx/vpx_encoder.h"
#include "third_party/libvpx/source/libvpx/vpx/vp8cx.h"
}

class SkBitmap;

namespace base {
class FilePath;
}

namespace media {

namespace chromeos {

class MEDIA_EXPORT WebmEncoder {
 public:
  
  
  WebmEncoder(const base::FilePath& output_path, int bitrate, bool realtime);
  ~WebmEncoder();

  
  
  
  bool EncodeFromSprite(const SkBitmap& sprite, int fps_n, int fps_d);

 private:
  
  
  bool WriteWebmHeader();

  
  void WriteWebmBlock(const vpx_codec_cx_pkt_t* packet);

  
  
  bool WriteWebmFooter();

  
  void StartSubElement(unsigned long class_id);

  
  void EndSubElement();

  
  void EbmlWrite(const void* buffer, unsigned long len);
  void EbmlSerialize(const void* buffer, int buffer_size, unsigned long len);

  template <typename T>
  void EbmlSerializeHelper(const T* buffer, unsigned long len);

  
  size_t width_;
  size_t height_;
  vpx_rational_t fps_;

  
  size_t frame_count_;

  
  vpx_codec_enc_cfg_t config_;

  
  int bitrate_;
  unsigned long deadline_;

  
  EbmlGlobal ebml_writer_;

  
  std::stack<long int> ebml_sub_elements_;

  base::FilePath output_path_;
  FILE* output_;

  
  bool has_errors_;

  DISALLOW_COPY_AND_ASSIGN(WebmEncoder);
};

}  

}  

#endif  
