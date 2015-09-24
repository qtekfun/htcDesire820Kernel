// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_IMAGE_WRITER_PRIVATE_IMAGE_WRITER_UTILS_H_
#define CHROME_BROWSER_EXTENSIONS_API_IMAGE_WRITER_PRIVATE_IMAGE_WRITER_UTILS_H_

#include "base/files/file_path.h"
#include "base/platform_file.h"

namespace extensions {
namespace image_writer_utils {

class ImageWriter {
 public:
  ImageWriter();
  virtual ~ImageWriter();

  
  virtual bool Open(const base::FilePath& path);
  virtual bool Close();

  // Writes from data_block to the device, and returns the amount written or -1
  
  virtual int Write(const char* data_block, int data_size);

 private:
   base::PlatformFile file_;
   int writes_count_;
};

class ImageReader {
 public:
  ImageReader();
  virtual ~ImageReader();

  
  virtual bool Open(const base::FilePath& path);
  virtual bool Close();
  
  virtual int Read(char* data_block, int data_size);
  virtual int64 GetSize();

 private:
  base::PlatformFile file_;
};

} 
} 

#endif  
