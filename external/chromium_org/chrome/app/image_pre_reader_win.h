// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_APP_IMAGE_PRE_READER_WIN_H_
#define CHROME_APP_IMAGE_PRE_READER_WIN_H_

#include "base/basictypes.h"

class ImagePreReader {
 public:
  
  
  
  
  
  
  
  
  static bool PreReadImage(const wchar_t* file_path,
                           size_t size_to_read,
                           size_t step_size);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool PartialPreReadImage(const wchar_t* file_path,
                                  size_t percentage,
                                  size_t max_chunk_size);

  
  
  
  static bool PartialPreReadImageOnDisk(const wchar_t* file_path,
                                        size_t percentage,
                                        size_t max_chunk_size);

  
  
  
  static bool PartialPreReadImageInMemory(const wchar_t* file_path,
                                          size_t percentage);
};  

#endif  
