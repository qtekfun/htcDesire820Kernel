// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_LOADER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_LOADER_H_
#pragma once

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/chromeos/login/image_decoder.h"

class MessageLoop;
class SkBitmap;

namespace chromeos {

class UserImageLoader : public base::RefCountedThreadSafe<UserImageLoader>,
                        public ImageDecoder::Delegate {
 public:
  class Delegate {
   public:
    
    
    
    virtual void OnImageLoaded(const std::string& username,
                               const SkBitmap& image,
                               bool should_save_image) = 0;

   protected:
    virtual ~Delegate() {}
  };

  explicit UserImageLoader(Delegate* delegate);

  
  
  void Start(const std::string& username,
             const std::string& filepath,
             bool should_save_image);

  void set_delegate(Delegate* delegate) { delegate_ = delegate; }

 private:
  friend class base::RefCountedThreadSafe<UserImageLoader>;

  
  struct ImageInfo {
    ImageInfo(const std::string& username, bool should_save)
        : username(username),
          should_save_image(should_save) {
    }

    std::string username;
    bool should_save_image;
  };

  typedef std::map<const ImageDecoder*, ImageInfo> ImageInfoMap;

  virtual ~UserImageLoader();

  
  
  void LoadImage(const std::string& filepath, const ImageInfo& image_info);

  
  virtual void OnImageDecoded(const ImageDecoder* decoder,
                              const SkBitmap& decoded_image);
  virtual void OnDecodeImageFailed(const ImageDecoder* decoder);

  
  void NotifyDelegate(const SkBitmap& image, const ImageInfo& image_info);

  
  MessageLoop* target_message_loop_;

  
  Delegate* delegate_;

  
  
  ImageInfoMap image_info_map_;

  DISALLOW_COPY_AND_ASSIGN(UserImageLoader);
};

}  

#endif  
