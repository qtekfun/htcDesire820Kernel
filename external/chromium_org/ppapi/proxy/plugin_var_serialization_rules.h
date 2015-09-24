// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PLUGIN_VAR_SERIALIZATION_RULES_H_
#define PPAPI_PROXY_PLUGIN_VAR_SERIALIZATION_RULES_H_

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "ppapi/proxy/var_serialization_rules.h"

namespace ppapi {
namespace proxy {

class PluginDispatcher;
class PluginVarTracker;

class PluginVarSerializationRules : public VarSerializationRules {
 public:
  
  
  explicit PluginVarSerializationRules(
      const base::WeakPtr<PluginDispatcher>& dispatcher);
  ~PluginVarSerializationRules();

  
  virtual PP_Var SendCallerOwned(const PP_Var& var);
  virtual PP_Var BeginReceiveCallerOwned(const PP_Var& var);
  virtual void EndReceiveCallerOwned(const PP_Var& var);
  virtual PP_Var ReceivePassRef(const PP_Var& var);
  virtual PP_Var BeginSendPassRef(const PP_Var& var);
  virtual void EndSendPassRef(const PP_Var& var);
  virtual void ReleaseObjectRef(const PP_Var& var);

 private:
  PluginVarTracker* var_tracker_;

  
  
  
  
  
  
  
  base::WeakPtr<PluginDispatcher> dispatcher_;

  DISALLOW_COPY_AND_ASSIGN(PluginVarSerializationRules);
};

}  
}  

#endif  
