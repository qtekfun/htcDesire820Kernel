// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_IMAGE_H_
#define PRINTING_IMAGE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "printing/printing_export.h"
#include "ui/gfx/size.h"

namespace base {
class FilePath;
}

namespace printing {

class Metafile;

class PRINTING_EXPORT Image {
 public:
  
  
  
  explicit Image(const base::FilePath& path);

  
  
  explicit Image(const Metafile& metafile);

  
  explicit Image(const Image& image);

  ~Image();

  const gfx::Size& size() const {
    return size_;
  }

  
  std::string checksum() const;

  
  bool SaveToPng(const base::FilePath& filepath) const;

  
  double PercentageDifferent(const Image& rhs) const;

  
  uint32 Color(uint32 color) const {
    if (ignore_alpha_)
      return color & 0xFFFFFF;  
    else
      return color;
  }

  uint32 pixel_at(int x, int y) const {
    DCHECK(x >= 0 && x < size_.width());
    DCHECK(y >= 0 && y < size_.height());
    const uint32* data = reinterpret_cast<const uint32*>(&*data_.begin());
    const uint32* data_row = data + y * row_length_ / sizeof(uint32);
    return Color(data_row[x]);
  }

 private:
  
  
  Image(const void* data, size_t size);

  bool LoadPng(const std::string& compressed);

  bool LoadMetafile(const std::string& data);

  bool LoadMetafile(const Metafile& metafile);

  
  gfx::Size size_;

  
  int row_length_;

  
  
  std::vector<unsigned char> data_;

  
  const bool ignore_alpha_;  

  
  Image& operator=(const Image& image);
};

}  

#endif  
