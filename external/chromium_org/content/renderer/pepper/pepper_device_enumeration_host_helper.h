// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PEPPER_DEVICE_ENUMERATION_HOST_HELPER_H_
#define CONTENT_RENDERER_PEPPER_PEPPER_DEVICE_ENUMERATION_HOST_HELPER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "ppapi/c/dev/ppb_device_ref_dev.h"
#include "url/gurl.h"

namespace ppapi {
struct DeviceRefData;

namespace host {
struct HostMessageContext;
struct ReplyMessageContext;
class ResourceHost;
}
}

namespace IPC {
class Message;
}

namespace content {

class CONTENT_EXPORT PepperDeviceEnumerationHostHelper {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    typedef base::Callback<
        void (int ,
              const std::vector<ppapi::DeviceRefData>& )>
        EnumerateDevicesCallback;

    
    
    virtual int EnumerateDevices(PP_DeviceType_Dev type,
                                 const GURL& document_url,
                                 const EnumerateDevicesCallback& callback) = 0;
    
    
    virtual void StopEnumerateDevices(int request_id) = 0;
  };

  
  PepperDeviceEnumerationHostHelper(ppapi::host::ResourceHost* resource_host,
                                    Delegate* delegate,
                                    PP_DeviceType_Dev device_type,
                                    const GURL& document_url);
  ~PepperDeviceEnumerationHostHelper();

  
  bool HandleResourceMessage(const IPC::Message& msg,
                             ppapi::host::HostMessageContext* context,
                             int32_t* result);

 private:
  class ScopedRequest;

  
  
  int32_t InternalHandleResourceMessage(
      const IPC::Message& msg,
      ppapi::host::HostMessageContext* context,
      bool* handled);

  int32_t OnEnumerateDevices(ppapi::host::HostMessageContext* context);
  int32_t OnMonitorDeviceChange(ppapi::host::HostMessageContext* context,
                                uint32_t callback_id);
  int32_t OnStopMonitoringDeviceChange(
      ppapi::host::HostMessageContext* context);

  void OnEnumerateDevicesComplete(
      int request_id,
      const std::vector<ppapi::DeviceRefData>& devices);
  void OnNotifyDeviceChange(
      uint32_t callback_id,
      int request_id,
      const std::vector<ppapi::DeviceRefData>& devices);

  
  ppapi::host::ResourceHost* resource_host_;
  Delegate* delegate_;

  PP_DeviceType_Dev device_type_;
  GURL document_url_;

  scoped_ptr<ScopedRequest> enumerate_;
  scoped_ptr<ScopedRequest> monitor_;

  scoped_ptr<ppapi::host::ReplyMessageContext> enumerate_devices_context_;

  DISALLOW_COPY_AND_ASSIGN(PepperDeviceEnumerationHostHelper);
};

}  

#endif  
