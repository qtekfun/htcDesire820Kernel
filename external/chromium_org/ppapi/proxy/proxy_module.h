// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PROXY_MODULE_H_
#define PPAPI_PROXY_PROXY_MODULE_H_

#include <string>

#include "base/basictypes.h"
#include "ppapi/proxy/ppapi_proxy_export.h"

template<typename T> struct DefaultSingletonTraits;

namespace ppapi {
namespace proxy {

class PPAPI_PROXY_EXPORT ProxyModule {
 public:
  
  static ProxyModule* GetInstance();

  
  
  
  const std::string& GetFlashCommandLineArgs();
  void SetFlashCommandLineArgs(const std::string& args);

 private:
  friend struct DefaultSingletonTraits<ProxyModule>;

  std::string flash_command_line_args_;

  ProxyModule();
  ~ProxyModule();

  DISALLOW_COPY_AND_ASSIGN(ProxyModule);
};

}  
}  

#endif  
