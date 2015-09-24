// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_PPB_URL_UTIL_SHARED_H_
#define PPAPI_SHARED_IMPL_PPB_URL_UTIL_SHARED_H_

#include <string>

#include "base/basictypes.h"
#include "ppapi/c/dev/ppb_url_util_dev.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"
#include "url/url_parse.h"

class GURL;

namespace ppapi {

class PPAPI_SHARED_EXPORT PPB_URLUtil_Shared {
 public:
  
  static PP_Var Canonicalize(PP_Var url,
                             PP_URLComponents_Dev* components);
  static PP_Var ResolveRelativeToURL(PP_Var base_url,
                                     PP_Var relative,
                                     PP_URLComponents_Dev* components);
  static PP_Bool IsSameSecurityOrigin(PP_Var url_a, PP_Var url_b);

  
  
  static PP_Var GenerateURLReturn(const GURL& url,
                                  PP_URLComponents_Dev* components);

  
  
  
  
  
  
  
  
  static PP_Var ConvertComponentsAndReturnURL(const PP_Var& url,
                                              PP_URLComponents_Dev* components);
};

}  

#endif  
