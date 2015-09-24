// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PPB_IMAGE_DATA_PROXY_H_
#define PPAPI_PPB_IMAGE_DATA_PROXY_H_

#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "build/build_config.h"
#include "ipc/ipc_platform_file.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/ppb_image_data.h"
#include "ppapi/proxy/interface_proxy.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/proxy/serialized_structs.h"
#include "ppapi/shared_impl/ppb_image_data_shared.h"
#include "ppapi/shared_impl/resource.h"
#include "ppapi/thunk/ppb_image_data_api.h"

class TransportDIB;

namespace ppapi {
namespace proxy {

class SerializedHandle;

class PPAPI_PROXY_EXPORT ImageData
    : public ppapi::Resource,
      public NON_EXPORTED_BASE(ppapi::thunk::PPB_ImageData_API),
      public ppapi::PPB_ImageData_Shared {
 public:
  virtual ~ImageData();

  
  virtual ppapi::thunk::PPB_ImageData_API* AsPPB_ImageData_API() OVERRIDE;
  virtual void LastPluginRefWasDeleted() OVERRIDE;
  virtual void InstanceWasDeleted() OVERRIDE;

  
  virtual PP_Bool Describe(PP_ImageDataDesc* desc) OVERRIDE;
  virtual int32_t GetSharedMemory(int* handle, uint32_t* byte_count) OVERRIDE;
  virtual void SetIsCandidateForReuse() OVERRIDE;

  PPB_ImageData_Shared::ImageDataType type() const { return type_; }
  const PP_ImageDataDesc& desc() const { return desc_; }

  
  
  void RecycleToPlugin(bool zero_contents);

 protected:
  ImageData(const ppapi::HostResource& resource,
            PPB_ImageData_Shared::ImageDataType type,
            const PP_ImageDataDesc& desc);

  PPB_ImageData_Shared::ImageDataType type_;
  PP_ImageDataDesc desc_;

  
  bool is_candidate_for_reuse_;

  DISALLOW_COPY_AND_ASSIGN(ImageData);
};

#if !defined(OS_NACL)
class PPAPI_PROXY_EXPORT PlatformImageData : public ImageData {
 public:
  PlatformImageData(const ppapi::HostResource& resource,
                    const PP_ImageDataDesc& desc,
                    ImageHandle handle);
  virtual ~PlatformImageData();

  
  virtual void* Map() OVERRIDE;
  virtual void Unmap() OVERRIDE;
  virtual SkCanvas* GetPlatformCanvas() OVERRIDE;
  virtual SkCanvas* GetCanvas() OVERRIDE;

  static ImageHandle NullHandle();
  static ImageHandle HandleFromInt(int32_t i);

 private:
  scoped_ptr<TransportDIB> transport_dib_;

  
  scoped_ptr<SkCanvas> mapped_canvas_;

  DISALLOW_COPY_AND_ASSIGN(PlatformImageData);
};
#endif  

class PPAPI_PROXY_EXPORT SimpleImageData : public ImageData {
 public:
  SimpleImageData(const ppapi::HostResource& resource,
                  const PP_ImageDataDesc& desc,
                  const base::SharedMemoryHandle& handle);
  virtual ~SimpleImageData();

  
  virtual void* Map() OVERRIDE;
  virtual void Unmap() OVERRIDE;
  virtual SkCanvas* GetPlatformCanvas() OVERRIDE;
  virtual SkCanvas* GetCanvas() OVERRIDE;

 private:
  base::SharedMemory shm_;
  uint32 size_;
  int map_count_;

  DISALLOW_COPY_AND_ASSIGN(SimpleImageData);
};

class PPB_ImageData_Proxy : public InterfaceProxy {
 public:
  PPB_ImageData_Proxy(Dispatcher* dispatcher);
  virtual ~PPB_ImageData_Proxy();

  static PP_Resource CreateProxyResource(
      PP_Instance instance,
      PPB_ImageData_Shared::ImageDataType type,
      PP_ImageDataFormat format,
      const PP_Size& size,
      PP_Bool init_to_zero);

  
  virtual bool OnMessageReceived(const IPC::Message& msg);

  
  
  
  // valid resource and the out params will be written.
  
  
  
  
  PPAPI_PROXY_EXPORT static PP_Resource CreateImageData(
      PP_Instance instance,
      PPB_ImageData_Shared::ImageDataType type,
      PP_ImageDataFormat format,
      const PP_Size& size,
      bool init_to_zero,
      PP_ImageDataDesc* desc,
      IPC::PlatformFileForTransit* image_handle,
      uint32_t* byte_count);

  static const ApiID kApiID = API_ID_PPB_IMAGE_DATA;

 private:
  
  void OnHostMsgCreatePlatform(
      PP_Instance instance,
      int32_t format,
      const PP_Size& size,
      PP_Bool init_to_zero,
      HostResource* result,
      PP_ImageDataDesc* desc,
      ImageHandle* result_image_handle);
  void OnHostMsgCreateSimple(
      PP_Instance instance,
      int32_t format,
      const PP_Size& size,
      PP_Bool init_to_zero,
      HostResource* result,
      PP_ImageDataDesc* desc,
      ppapi::proxy::SerializedHandle* result_image_handle);

  
  void OnPluginMsgNotifyUnusedImageData(const HostResource& old_image_data);

  DISALLOW_COPY_AND_ASSIGN(PPB_ImageData_Proxy);
};

}  
}  

#endif  
