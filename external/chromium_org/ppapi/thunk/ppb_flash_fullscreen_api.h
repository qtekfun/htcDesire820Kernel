// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_THUNK_PPB_FLASH_FULLSCREEN_API_H_
#define PPAPI_THUNK_PPB_FLASH_FULLSCREEN_API_H_

#include "ppapi/shared_impl/singleton_resource_id.h"
#include "ppapi/thunk/ppapi_thunk_export.h"

namespace ppapi {
namespace thunk {

class PPAPI_THUNK_EXPORT PPB_Flash_Fullscreen_API {
 public:
  virtual ~PPB_Flash_Fullscreen_API() {}

  virtual PP_Bool IsFullscreen(PP_Instance instance) = 0;
  virtual PP_Bool SetFullscreen(PP_Instance instance,
                                PP_Bool fullscreen) = 0;

  
  
  
  virtual void SetLocalIsFullscreen(PP_Instance instance,
                                    PP_Bool fullscreen) = 0;

  static const SingletonResourceID kSingletonResourceID =
      FLASH_FULLSCREEN_SINGLETON_ID;
};

}  
}  

#endif 
