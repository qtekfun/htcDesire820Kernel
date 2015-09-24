// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_H_

#include <vector>

#include "ui/gfx/image/image_skia.h"
#include "url/gurl.h"

namespace chromeos {

class UserImage {
 public:
  
  typedef std::vector<unsigned char> RawImage;

  
  
  
  static UserImage CreateAndEncode(const gfx::ImageSkia& image);

  
  UserImage();

  
  explicit UserImage(const gfx::ImageSkia& image);

  
  
  
  
  UserImage(const gfx::ImageSkia& image, const RawImage& raw_image);

  virtual ~UserImage();

  const gfx::ImageSkia& image() const { return image_; }

  
  bool has_raw_image() const { return has_raw_image_; }
  const RawImage& raw_image() const { return raw_image_; }

  
  void DiscardRawImage();

  
  bool has_animated_image() const { return has_animated_image_; }
  const RawImage& animated_image() const { return animated_image_; }

  
  void set_url(const GURL& url) { url_ = url; }
  GURL url() const { return url_; }

  
  
  bool is_safe_format() const { return is_safe_format_; }
  void MarkAsSafe();

 private:
  gfx::ImageSkia image_;
  bool has_raw_image_;
  RawImage raw_image_;
  bool has_animated_image_;
  RawImage animated_image_;
  GURL url_;
  bool is_safe_format_;
};

}  

#endif  
