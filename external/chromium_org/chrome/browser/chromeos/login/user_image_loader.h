// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_LOADER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_LOADER_H_

#include <map>
#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/image_decoder.h"

class SkBitmap;

namespace base {
class SequencedTaskRunner;
}

namespace chromeos {

class UserImage;

class UserImageLoader : public base::RefCountedThreadSafe<UserImageLoader>,
                        public ImageDecoder::Delegate {
 public:
  
  typedef base::Callback<void(const UserImage& user_image)> LoadedCallback;

  
  UserImageLoader(
      ImageDecoder::ImageCodec image_codec,
      scoped_refptr<base::SequencedTaskRunner> background_task_runner);

  
  
  
  
  
  void Start(const std::string& filepath,
             int size,
             const LoadedCallback& loaded_cb);
  void Start(scoped_ptr<std::string> data,
             int size,
             const LoadedCallback& loaded_cb);

 private:
  friend class base::RefCountedThreadSafe<UserImageLoader>;

  
  struct ImageInfo {
    ImageInfo(int size, const LoadedCallback& loaded_cb);
    ~ImageInfo();

    const int size;
    const LoadedCallback loaded_cb;
  };

  typedef std::map<const ImageDecoder*, ImageInfo> ImageInfoMap;

  virtual ~UserImageLoader();

  
  
  void ReadAndDecodeImage(const std::string& filepath,
                          const ImageInfo& image_info);

  
  
  void DecodeImage(const scoped_ptr<std::string> data,
                   const ImageInfo& image_info);

  
  
  virtual void OnImageDecoded(const ImageDecoder* decoder,
                              const SkBitmap& decoded_image) OVERRIDE;
  virtual void OnDecodeImageFailed(const ImageDecoder* decoder) OVERRIDE;

  
  
  scoped_refptr<base::SequencedTaskRunner> foreground_task_runner_;

  
  
  scoped_refptr<base::SequencedTaskRunner> background_task_runner_;

  
  const ImageDecoder::ImageCodec image_codec_;

  
  
  ImageInfoMap image_info_map_;

  DISALLOW_COPY_AND_ASSIGN(UserImageLoader);
};

}  

#endif  
