/*
 * Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_SRPC_CLIENT_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_SRPC_CLIENT_H_

#include <map>
#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/nacl_string.h"
#include "native_client/src/shared/srpc/nacl_srpc.h"
#include "ppapi/native_client/src/trusted/plugin/utility.h"

namespace nacl {
class DescWrapper;
}  

namespace plugin {

class ErrorInfo;
class MethodInfo;
class Plugin;
class SrpcParams;

class SrpcClient {
 public:
  
  static SrpcClient* New(nacl::DescWrapper* wrapper);

  
  
  bool Init(nacl::DescWrapper* socket);

  
  ~SrpcClient();

  
  bool HasMethod(const nacl::string& method_name);
  
  bool Invoke(const nacl::string& method_name, SrpcParams* params);
  
  NaClSrpcError GetLastError() { return last_error_; }
  bool InitParams(const nacl::string& method_name, SrpcParams* params);

  
  void AttachService(NaClSrpcService* service, void* instance_data);

 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(SrpcClient);
  SrpcClient();
  void GetMethods();
  typedef std::map<nacl::string, MethodInfo*> Methods;
  Methods methods_;
  NaClSrpcChannel srpc_channel_;
  bool srpc_channel_initialised_;
  NaClSrpcError last_error_;
};

}  

#endif  
