// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_IMAGE_DECODER_TEST_H_
#define CONTENT_TEST_IMAGE_DECODER_TEST_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"

#include "testing/gtest/include/gtest/gtest.h"

namespace blink { class WebImageDecoder; }



enum ImageDecoderTestFileSelection {
  TEST_ALL,
  TEST_SMALLER,
  TEST_BIGGER,
};

base::FilePath GetMD5SumPath(const base::FilePath& path);

class ImageDecoderTest : public testing::Test {
 public:
  explicit ImageDecoderTest(const std::string& format) : format_(format) { }

 protected:
  virtual void SetUp() OVERRIDE;

  
  std::vector<base::FilePath> GetImageFiles() const;

  
  bool ShouldImageFail(const base::FilePath& path) const;

  
  
  
  void TestWebKitImageDecoder(const base::FilePath& image_path,
    const base::FilePath& md5_sum_path, int desired_frame_index) const;

  
  
  
  
  void TestDecoding(ImageDecoderTestFileSelection file_selection,
                    const int64 threshold);

  void TestDecoding()  {
    TestDecoding(TEST_ALL, 0);
  }

  
  virtual blink::WebImageDecoder* CreateWebKitImageDecoder() const = 0;

  
  std::string format_;

 protected:
  
  base::FilePath data_dir_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ImageDecoderTest);
};

#endif  
