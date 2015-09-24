// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PROXY_OBJECT_VAR_H_
#define PPAPI_PROXY_PROXY_OBJECT_VAR_H_

#include "base/compiler_specific.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/shared_impl/var.h"

namespace ppapi {

namespace proxy {
class PluginDispatcher;
}  

class PPAPI_PROXY_EXPORT ProxyObjectVar : public Var {
 public:
  ProxyObjectVar(proxy::PluginDispatcher* dispatcher,
                 int32 host_var_id);

  virtual ~ProxyObjectVar();

  
  virtual ProxyObjectVar* AsProxyObjectVar() OVERRIDE;
  virtual PP_VarType GetType() const OVERRIDE;

  proxy::PluginDispatcher* dispatcher() const { return dispatcher_; }
  int32 host_var_id() const { return host_var_id_; }

  void* user_data() const { return user_data_; }
  void set_user_data(void* ud) { user_data_ = ud; }

  
  
  void AssignVarID(int32 id);

  void clear_dispatcher() { dispatcher_ = NULL; }

 private:
  proxy::PluginDispatcher* dispatcher_;
  int32 host_var_id_;

  
  
  
  void* user_data_;

  DISALLOW_COPY_AND_ASSIGN(ProxyObjectVar);
};

}  

#endif  
