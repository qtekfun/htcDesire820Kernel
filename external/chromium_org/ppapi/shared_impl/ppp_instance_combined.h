// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_PPP_INSTANCE_COMBINED_H_
#define PPAPI_SHARED_IMPL_PPP_INSTANCE_COMBINED_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "ppapi/c/ppp_instance.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

struct PPAPI_SHARED_EXPORT PPP_Instance_Combined {
 public:
  
  
  
  
  static PPP_Instance_Combined* Create(
      base::Callback<const void*(const char*)> get_plugin_if);

  PP_Bool DidCreate(PP_Instance instance,
                    uint32_t argc,
                    const char* argn[],
                    const char* argv[]);
  void DidDestroy(PP_Instance instance);

  
  
  
  
  void DidChangeView(PP_Instance instance,
                     PP_Resource view_changed_resource,
                     const struct PP_Rect* position,
                     const struct PP_Rect* clip);

  void DidChangeFocus(PP_Instance instance, PP_Bool has_focus);
  PP_Bool HandleDocumentLoad(PP_Instance instance, PP_Resource url_loader);

 private:
  explicit PPP_Instance_Combined(const PPP_Instance_1_0& instance_if);
  explicit PPP_Instance_Combined(const PPP_Instance_1_1& instance_if);

  
  
  PPP_Instance_1_1 instance_1_1_;

  
  void (*did_change_view_1_0_)(PP_Instance instance,
                               const struct PP_Rect* position,
                               const struct PP_Rect* clip);

  DISALLOW_COPY_AND_ASSIGN(PPP_Instance_Combined);
};

}  

#endif  

