// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_PPB_IMAGE_DATA_SHARED_H_
#define PPAPI_SHARED_IMPL_PPB_IMAGE_DATA_SHARED_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/ppb_image_data.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

class PPAPI_SHARED_EXPORT PPB_ImageData_Shared {
 public:
  enum ImageDataType {
    
    
    
    
    PLATFORM,
    
    
    
    SIMPLE
  };

  static PP_ImageDataFormat GetNativeImageDataFormat();
  static PP_Bool IsImageDataFormatSupported(PP_ImageDataFormat format);
  static PP_Bool IsImageDataDescValid(const PP_ImageDataDesc& desc);
};

}  

#endif  
