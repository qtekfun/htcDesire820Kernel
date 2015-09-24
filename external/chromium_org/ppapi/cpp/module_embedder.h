// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_MODULE_EMBEDDER_H_
#define PPAPI_CPP_MODULE_EMBEDDER_H_

#include "ppapi/c/ppp.h"

namespace pp {

class Module;

pp::Module* CreateModule();

void SetBrokerGetInterfaceFunc(PP_GetInterface_Func broker_get_interface);

}  

#endif  
