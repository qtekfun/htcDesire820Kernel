// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_SCRIPTABLE_PLUGIN_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_SCRIPTABLE_PLUGIN_H_

#include <vector>

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/portability.h"
#include "ppapi/cpp/dev/scriptable_object_deprecated.h"
#include "ppapi/cpp/private/var_private.h"
#include "ppapi/native_client/src/trusted/plugin/utility.h"

namespace plugin {

class Plugin;

class ScriptablePlugin : public pp::deprecated::ScriptableObject {
 public:
  
  static ScriptablePlugin* NewPlugin(Plugin* plugin);

  
  pp::VarPrivate* var() { return var_; }

  static void Unref(ScriptablePlugin** handle);

  
  Plugin* plugin() const { return plugin_; }

  
  
  
  ScriptablePlugin* AddRef();
  
  
  
  
  void Unref();

  

  
  
  virtual bool HasProperty(const pp::Var& name, pp::Var* exception);
  
  
  virtual bool HasMethod(const pp::Var& name, pp::Var* exception);

  
  
  virtual pp::Var GetProperty(const pp::Var& name, pp::Var* exception);
  
  
  virtual void SetProperty(const pp::Var& name, const pp::Var& value,
                           pp::Var* exception);
  
  virtual void RemoveProperty(const pp::Var& name, pp::Var* exception);
  
  
  virtual void GetAllPropertyNames(std::vector<pp::Var>* properties,
                                   pp::Var* exception);

  
  
  virtual pp::Var Call(const pp::Var& name, const std::vector<pp::Var>& args,
                       pp::Var* exception);

  
  virtual pp::Var Construct(const std::vector<pp::Var>& args,
                            pp::Var* exception);

 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(ScriptablePlugin);
  
  
  explicit ScriptablePlugin(Plugin* plugin);
  
  
  virtual ~ScriptablePlugin();

  
  
  
  
  
  
  
  
  
  
  pp::VarPrivate* var_;

  
  
  int num_unref_calls_;

  
  Plugin* plugin_;
};

}  

#endif  
