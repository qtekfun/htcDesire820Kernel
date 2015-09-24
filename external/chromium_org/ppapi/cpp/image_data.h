// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_IMAGE_DATA_H_
#define PPAPI_CPP_IMAGE_DATA_H_

#include "ppapi/c/ppb_image_data.h"
#include "ppapi/cpp/point.h"
#include "ppapi/cpp/size.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class InstanceHandle;

class ImageData : public Resource {
 public:
  
  
  ImageData();

  
  
  
  
  ImageData(PassRef, PP_Resource resource);

  
  
  
  
  
  ImageData(const ImageData& other);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  ImageData(const InstanceHandle& instance,
            PP_ImageDataFormat format,
            const Size& size,
            bool init_to_zero);

  
  
  
  
  
  
  
  
  ImageData& operator=(const ImageData& other);

  
  
  
  
  
  
  
  
  
  
  static bool IsImageDataFormatSupported(PP_ImageDataFormat format);

  
  
  
  
  
  static PP_ImageDataFormat GetNativeImageDataFormat();

  
  
  
  PP_ImageDataFormat format() const { return desc_.format; }

  
  
  
  pp::Size size() const { return desc_.size; }

  
  
  
  int32_t stride() const { return desc_.stride; }

  
  
  
  void* data() const { return data_; }

  
  
  
  
  
  
  
  const uint32_t* GetAddr32(const Point& coord) const;

  
  
  
  
  
  
  
  uint32_t* GetAddr32(const Point& coord);

 private:
  void InitData();

  PP_ImageDataDesc desc_;
  void* data_;
};

}  

#endif  
