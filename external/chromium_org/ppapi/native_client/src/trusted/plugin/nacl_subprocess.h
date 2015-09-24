// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_NACL_SUBPROCESS_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_NACL_SUBPROCESS_H_

#include <stdarg.h>

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/nacl_string.h"
#include "native_client/src/include/portability.h"

#include "ppapi/native_client/src/trusted/plugin/service_runtime.h"
#include "ppapi/native_client/src/trusted/plugin/srpc_client.h"

namespace plugin {

class Plugin;
class ServiceRuntime;
class SrpcParams;


class NaClSubprocess {
 public:
  NaClSubprocess(const nacl::string& description,
                 ServiceRuntime* service_runtime,
                 SrpcClient* srpc_client)
    : description_(description),
      service_runtime_(service_runtime),
      srpc_client_(srpc_client) {
  }
  virtual ~NaClSubprocess();

  ServiceRuntime* service_runtime() const { return service_runtime_.get(); }
  void set_service_runtime(ServiceRuntime* service_runtime) {
    service_runtime_.reset(service_runtime);
  }

  
  SrpcClient* srpc_client() const { return srpc_client_.get(); }

  
  nacl::string description() const { return description_; }

  
  
  nacl::string detailed_description() const;

  
  bool StartSrpcServices();

  
  
  
  
  
  bool InvokeSrpcMethod(const nacl::string& method_name,
                        const nacl::string& input_signature,
                        SrpcParams* out_params,
                        ...);

  
  void Shutdown();

 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(NaClSubprocess);

  bool VInvokeSrpcMethod(const nacl::string& method_name,
                         const nacl::string& signature,
                         SrpcParams* params,
                         va_list vl);

  nacl::string description_;

  
  nacl::scoped_ptr<ServiceRuntime> service_runtime_;
  
  nacl::scoped_ptr<SrpcClient> srpc_client_;
};

}  

#endif  
