// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_RESOURCE_H_
#define PPAPI_SHARED_IMPL_RESOURCE_H_

#include <stddef.h>  

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/ppb_console.h"
#include "ppapi/shared_impl/host_resource.h"

#define FOR_ALL_PPAPI_RESOURCE_APIS(F) \
  F(ExtensionsCommon_API) \
  F(PPB_Audio_API) \
  F(PPB_AudioConfig_API) \
  F(PPB_AudioInput_API) \
  F(PPB_AudioTrusted_API) \
  F(PPB_Broker_API) \
  F(PPB_Broker_Instance_API) \
  F(PPB_BrowserFont_Singleton_API) \
  F(PPB_BrowserFont_Trusted_API) \
  F(PPB_Buffer_API) \
  F(PPB_DeviceRef_API) \
  F(PPB_Ext_CrxFileSystem_Private_API) \
  F(PPB_FileChooser_API) \
  F(PPB_FileIO_API) \
  F(PPB_FileRef_API) \
  F(PPB_FileSystem_API) \
  F(PPB_Find_API) \
  F(PPB_Flash_Clipboard_API) \
  F(PPB_Flash_DRM_API) \
  F(PPB_Flash_File_API) \
  F(PPB_Flash_FontFile_API) \
  F(PPB_Flash_Fullscreen_API) \
  F(PPB_Flash_Functions_API) \
  F(PPB_Flash_Menu_API) \
  F(PPB_Flash_MessageLoop_API) \
  F(PPB_Gamepad_API) \
  F(PPB_Graphics2D_API) \
  F(PPB_Graphics3D_API) \
  F(PPB_HostResolver_API) \
  F(PPB_HostResolver_Private_API) \
  F(PPB_ImageData_API) \
  F(PPB_InputEvent_API) \
  F(PPB_IsolatedFileSystem_Private_API) \
  F(PPB_LayerCompositor_API) \
  F(PPB_MessageLoop_API) \
  F(PPB_NetAddress_API) \
  F(PPB_NetworkList_API) \
  F(PPB_NetworkMonitor_API) \
  F(PPB_NetworkProxy_API) \
  F(PPB_OutputProtection_API) \
  F(PPB_PDF_API) \
  F(PPB_PlatformVerification_API) \
  F(PPB_Printing_API) \
  F(PPB_ResourceArray_API) \
  F(PPB_Scrollbar_API) \
  F(PPB_Talk_Private_API) \
  F(PPB_TrueTypeFont_API) \
  F(PPB_TrueTypeFont_Singleton_API) \
  F(PPB_TCPServerSocket_Private_API) \
  F(PPB_TCPSocket_API) \
  F(PPB_TCPSocket_Private_API) \
  F(PPB_UDPSocket_API) \
  F(PPB_UDPSocket_Private_API) \
  F(PPB_URLLoader_API) \
  F(PPB_URLRequestInfo_API) \
  F(PPB_URLResponseInfo_API) \
  F(PPB_VideoCapture_API) \
  F(PPB_VideoDecoder_API) \
  F(PPB_VideoDestination_Private_API) \
  F(PPB_VideoLayer_API) \
  F(PPB_VideoSource_Private_API) \
  F(PPB_View_API) \
  F(PPB_WebSocket_API) \
  F(PPB_Widget_API) \
  F(PPB_X509Certificate_Private_API)

namespace IPC {
class Message;
}

namespace ppapi {

namespace proxy {
class ResourceMessageReplyParams;
}

namespace thunk {
#define DECLARE_RESOURCE_CLASS(RESOURCE) class RESOURCE;
FOR_ALL_PPAPI_RESOURCE_APIS(DECLARE_RESOURCE_CLASS)
#undef DECLARE_RESOURCE_CLASS
}  

enum ResourceObjectType {
  OBJECT_IS_IMPL,
  OBJECT_IS_PROXY
};

class PPAPI_SHARED_EXPORT Resource : public base::RefCounted<Resource> {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  Resource(ResourceObjectType type, PP_Instance instance);

  
  
  
  
  
  
  
  
  
  
  
  Resource(ResourceObjectType type, const HostResource& host_resource);

  
  
  
  
  struct Untracked {};
  explicit Resource(Untracked);

  virtual ~Resource();

  PP_Instance pp_instance() const { return host_resource_.instance(); }

  
  
  PP_Resource pp_resource() const { return pp_resource_; }

  
  
  
  const HostResource& host_resource() { return host_resource_; }

  
  
  
  
  PP_Resource GetReference();

  
  
  
  
  
  
  
  virtual void NotifyLastPluginRefWasDeleted();

  
  
  
  
  
  
  
  
  
  
  
  virtual void NotifyInstanceWasDeleted();

  
  
  
  #define DEFINE_TYPE_GETTER(RESOURCE) \
    virtual thunk::RESOURCE* As##RESOURCE();
  FOR_ALL_PPAPI_RESOURCE_APIS(DEFINE_TYPE_GETTER)
  #undef DEFINE_TYPE_GETTER

  
  
  
  
  template <typename T> T* GetAs();

  
  
  
  
  
  
  
  
  
  
  virtual void OnReplyReceived(const proxy::ResourceMessageReplyParams& params,
                               const IPC::Message& msg);

 protected:
  
  void Log(PP_LogLevel level, const std::string& message);

  
  
  
  
  
  
  void RemoveFromResourceTracker();

  
  virtual void LastPluginRefWasDeleted() {}
  virtual void InstanceWasDeleted() {}

 private:
  
  PP_Resource pp_resource_;
  HostResource host_resource_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(Resource);
};

#define DEFINE_RESOURCE_CAST(RESOURCE) \
  template<> inline thunk::RESOURCE* Resource::GetAs() { \
    return As##RESOURCE(); \
  }
FOR_ALL_PPAPI_RESOURCE_APIS(DEFINE_RESOURCE_CAST)
#undef DEFINE_RESOURCE_CAST

}  

#endif  
