// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMAGE_DECODER_H_
#define CHROME_BROWSER_IMAGE_DECODER_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/threading/sequenced_worker_pool.h"
#include "content/public/browser/utility_process_host_client.h"

class SkBitmap;

class ImageDecoder : public content::UtilityProcessHostClient {
 public:
  class Delegate {
   public:
    
    
    
    virtual void OnImageDecoded(const ImageDecoder* decoder,
                                const SkBitmap& decoded_image) = 0;

    
    
    virtual void OnDecodeImageFailed(const ImageDecoder* decoder) {}

   protected:
    virtual ~Delegate() {}
  };

  enum ImageCodec {
    DEFAULT_CODEC = 0,  
    ROBUST_JPEG_CODEC,  
  };

  ImageDecoder(Delegate* delegate,
               const std::string& image_data,
               ImageCodec image_codec);

  
  
  void Start(scoped_refptr<base::SequencedTaskRunner> task_runner);

  const std::vector<unsigned char>& get_image_data() const {
    return image_data_;
  }

  void set_delegate(Delegate* delegate) { delegate_ = delegate; }

 private:
  
  virtual ~ImageDecoder();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  void OnDecodeImageSucceeded(const SkBitmap& decoded_image);
  void OnDecodeImageFailed();

  
  void DecodeImageInSandbox(const std::vector<unsigned char>& image_data);

  Delegate* delegate_;
  std::vector<unsigned char> image_data_;
  const ImageCodec image_codec_;
  scoped_refptr<base::SequencedTaskRunner> task_runner_;

  DISALLOW_COPY_AND_ASSIGN(ImageDecoder);
};

#endif  
