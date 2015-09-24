// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_IMAGE_DECODER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_IMAGE_DECODER_H_
#pragma once

#include <string>
#include <vector>

#include "chrome/browser/utility_process_host.h"

namespace chromeos {

class ImageDecoder : public UtilityProcessHost::Client {
 public:
  class Delegate {
   public:
    
    
    
    virtual void OnImageDecoded(const ImageDecoder* decoder,
                                const SkBitmap& decoded_image) = 0;

    
    
    virtual void OnDecodeImageFailed(const ImageDecoder* decoder) {}

   protected:
    virtual ~Delegate() {}
  };

  ImageDecoder(Delegate* delegate,
               const std::string& image_data);

  
  void Start();

 private:
  
  virtual ~ImageDecoder() {}

  
  virtual void OnDecodeImageSucceeded(const SkBitmap& decoded_image);
  virtual void OnDecodeImageFailed();

  
  void DecodeImageInSandbox(const std::vector<unsigned char>& image_data);

  Delegate* delegate_;
  std::vector<unsigned char> image_data_;
  BrowserThread::ID target_thread_id_;

  DISALLOW_COPY_AND_ASSIGN(ImageDecoder);
};

}  

#endif  
