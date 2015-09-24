// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_ISOLATED_FILE_SYSTEM_PRIVATE_RESOURCE_H_
#define PPAPI_PROXY_ISOLATED_FILE_SYSTEM_PRIVATE_RESOURCE_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "ppapi/proxy/connection.h"
#include "ppapi/proxy/plugin_resource.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/thunk/ppb_isolated_file_system_private_api.h"

namespace ppapi {

class TrackedCallback;

namespace proxy {

class ResourceMessageReplyParams;

class PPAPI_PROXY_EXPORT IsolatedFileSystemPrivateResource
    : public PluginResource,
      public thunk::PPB_IsolatedFileSystem_Private_API {
 public:
  IsolatedFileSystemPrivateResource(
      Connection connection, PP_Instance instance);
  virtual ~IsolatedFileSystemPrivateResource();

  
  virtual thunk::PPB_IsolatedFileSystem_Private_API*
      AsPPB_IsolatedFileSystem_Private_API() OVERRIDE;

  
  virtual int32_t Open(PP_Instance instance,
                       PP_IsolatedFileSystemType_Private type,
                       PP_Resource* file_system_resource,
                       scoped_refptr<TrackedCallback> callback) OVERRIDE;

 private:
  void OnBrowserOpenComplete(PP_IsolatedFileSystemType_Private type,
                             PP_Resource* file_system_resource,
                             scoped_refptr<TrackedCallback> callback,
                             const ResourceMessageReplyParams& params,
                             const std::string& fsid);

  DISALLOW_COPY_AND_ASSIGN(IsolatedFileSystemPrivateResource);
};

}  
}  

#endif  
