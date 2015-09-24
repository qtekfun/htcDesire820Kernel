// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PPB_IMAGE_DATA_IMPL_H_
#define CONTENT_RENDERER_PEPPER_PPB_IMAGE_DATA_IMPL_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "ppapi/c/ppb_image_data.h"
#include "ppapi/shared_impl/ppb_image_data_shared.h"
#include "ppapi/shared_impl/resource.h"
#include "ppapi/thunk/ppb_image_data_api.h"
#include "third_party/skia/include/core/SkCanvas.h"

class SkBitmap;
class SkCanvas;
class TransportDIB;

namespace base {
class SharedMemory;
}

namespace content {

class CONTENT_EXPORT PPB_ImageData_Impl
    : public ppapi::Resource,
      public ppapi::PPB_ImageData_Shared,
      public NON_EXPORTED_BASE(ppapi::thunk::PPB_ImageData_API) {
 public:
  
  
  
  
  class Backend {
   public:
    virtual ~Backend() {};
    virtual bool Init(PPB_ImageData_Impl* impl, PP_ImageDataFormat format,
                      int width, int height, bool init_to_zero) = 0;
    virtual bool IsMapped() const = 0;
    virtual TransportDIB* GetTransportDIB() const = 0;
    virtual void* Map() = 0;
    virtual void Unmap() = 0;
    virtual int32_t GetSharedMemory(int* handle, uint32_t* byte_count) = 0;
    virtual SkCanvas* GetPlatformCanvas() = 0;
    virtual SkCanvas* GetCanvas() = 0;
    virtual const SkBitmap* GetMappedBitmap() const = 0;
  };

  
  
  
  PPB_ImageData_Impl(PP_Instance instance,
                     PPB_ImageData_Shared::ImageDataType type);

  
  struct ForTest {};
  PPB_ImageData_Impl(PP_Instance instance,
                     ForTest);

  bool Init(PP_ImageDataFormat format,
            int width, int height,
            bool init_to_zero);

  static PP_Resource Create(PP_Instance pp_instance,
                            PPB_ImageData_Shared::ImageDataType type,
                            PP_ImageDataFormat format,
                            const PP_Size& size,
                            PP_Bool init_to_zero);

  int width() const { return width_; }
  int height() const { return height_; }

  
  PP_ImageDataFormat format() const { return format_; }

  
  
  bool IsMapped() const;
  TransportDIB* GetTransportDIB() const;

  
  virtual ppapi::thunk::PPB_ImageData_API* AsPPB_ImageData_API() OVERRIDE;

  
  virtual PP_Bool Describe(PP_ImageDataDesc* desc) OVERRIDE;
  virtual void* Map() OVERRIDE;
  virtual void Unmap() OVERRIDE;
  virtual int32_t GetSharedMemory(int* handle, uint32_t* byte_count) OVERRIDE;
  virtual SkCanvas* GetPlatformCanvas() OVERRIDE;
  virtual SkCanvas* GetCanvas() OVERRIDE;
  virtual void SetIsCandidateForReuse() OVERRIDE;

  const SkBitmap* GetMappedBitmap() const;

 private:
  virtual ~PPB_ImageData_Impl();

  PP_ImageDataFormat format_;
  int width_;
  int height_;
  scoped_ptr<Backend> backend_;

  DISALLOW_COPY_AND_ASSIGN(PPB_ImageData_Impl);
};

class ImageDataPlatformBackend : public PPB_ImageData_Impl::Backend {
 public:
  
  
  ImageDataPlatformBackend(bool is_browser_allocated);
  virtual ~ImageDataPlatformBackend();

  
  virtual bool Init(PPB_ImageData_Impl* impl, PP_ImageDataFormat format,
                    int width, int height, bool init_to_zero) OVERRIDE;
  virtual bool IsMapped() const OVERRIDE;
  virtual TransportDIB* GetTransportDIB() const OVERRIDE;
  virtual void* Map() OVERRIDE;
  virtual void Unmap() OVERRIDE;
  virtual int32_t GetSharedMemory(int* handle, uint32_t* byte_count) OVERRIDE;
  virtual SkCanvas* GetPlatformCanvas() OVERRIDE;
  virtual SkCanvas* GetCanvas() OVERRIDE;
  virtual const SkBitmap* GetMappedBitmap() const OVERRIDE;

 private:
  
  
  int width_;
  int height_;
  scoped_ptr<TransportDIB> dib_;

  bool is_browser_allocated_;

  
  scoped_ptr<SkCanvas> mapped_canvas_;

  DISALLOW_COPY_AND_ASSIGN(ImageDataPlatformBackend);
};

class ImageDataSimpleBackend : public PPB_ImageData_Impl::Backend {
 public:
  ImageDataSimpleBackend();
  virtual ~ImageDataSimpleBackend();

  
  virtual bool Init(PPB_ImageData_Impl* impl, PP_ImageDataFormat format,
            int width, int height, bool init_to_zero) OVERRIDE;
  virtual bool IsMapped() const OVERRIDE;
  virtual TransportDIB* GetTransportDIB() const OVERRIDE;
  virtual void* Map() OVERRIDE;
  virtual void Unmap() OVERRIDE;
  virtual int32_t GetSharedMemory(int* handle, uint32_t* byte_count) OVERRIDE;
  virtual SkCanvas* GetPlatformCanvas() OVERRIDE;
  virtual SkCanvas* GetCanvas() OVERRIDE;
  virtual const SkBitmap* GetMappedBitmap() const OVERRIDE;

 private:
  scoped_ptr<base::SharedMemory> shared_memory_;
  
  SkBitmap skia_bitmap_;
  scoped_ptr<SkCanvas> skia_canvas_;
  uint32 map_count_;

  DISALLOW_COPY_AND_ASSIGN(ImageDataSimpleBackend);
};

class ImageDataAutoMapper {
 public:
  explicit ImageDataAutoMapper(PPB_ImageData_Impl* image_data)
        : image_data_(image_data) {
    if (image_data_->IsMapped()) {
      is_valid_ = true;
      needs_unmap_ = false;
    } else {
      is_valid_ = needs_unmap_ = !!image_data_->Map();
    }
  }

  ~ImageDataAutoMapper() {
    if (needs_unmap_)
      image_data_->Unmap();
  }

  
  
  bool is_valid() const { return is_valid_; }

 private:
  PPB_ImageData_Impl* image_data_;
  bool is_valid_;
  bool needs_unmap_;

  DISALLOW_COPY_AND_ASSIGN(ImageDataAutoMapper);
};

}  

#endif  
