// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_URL_RESPONSE_INFO_RESOURCE_H_
#define PPAPI_PROXY_URL_RESPONSE_INFO_RESOURCE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ppapi/proxy/plugin_resource.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/shared_impl/scoped_pp_resource.h"
#include "ppapi/shared_impl/url_response_info_data.h"
#include "ppapi/thunk/ppb_url_response_info_api.h"

namespace ppapi {
namespace proxy {

class PPAPI_PROXY_EXPORT URLResponseInfoResource
    : public PluginResource,
      public NON_EXPORTED_BASE(thunk::PPB_URLResponseInfo_API) {
 public:
  
  
  
  
  URLResponseInfoResource(Connection connection,
                          PP_Instance instance,
                          const URLResponseInfoData& data,
                          PP_Resource file_ref_resource);
  virtual ~URLResponseInfoResource();

  
  virtual PPB_URLResponseInfo_API* AsPPB_URLResponseInfo_API() OVERRIDE;

  
  virtual PP_Var GetProperty(PP_URLResponseProperty property) OVERRIDE;
  virtual PP_Resource GetBodyAsFileRef() OVERRIDE;

  const URLResponseInfoData& data() const { return data_; }

 private:
  URLResponseInfoData data_;

  
  ScopedPPResource body_as_file_ref_;

  DISALLOW_COPY_AND_ASSIGN(URLResponseInfoResource);
};

}  
}  

#endif  
