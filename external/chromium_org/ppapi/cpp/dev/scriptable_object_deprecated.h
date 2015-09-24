// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_SCRIPTABLE_OBJECT_DEPRECATED_H_
#define PPAPI_CPP_SCRIPTABLE_OBJECT_DEPRECATED_H_

#include <vector>

struct PPP_Class_Deprecated;

namespace pp {
class Var;
class VarPrivate;
}

namespace pp {

namespace deprecated {

class ScriptableObject {
 public:
  ScriptableObject() {}
  virtual ~ScriptableObject() {}

  
  virtual bool HasProperty(const Var& name, Var* exception);

  
  virtual bool HasMethod(const Var& name, Var* exception);

  
  
  virtual Var GetProperty(const Var& name, Var* exception);

  
  virtual void GetAllPropertyNames(std::vector<Var>* properties,
                                   Var* exception);

  
  
  virtual void SetProperty(const Var& name,
                           const Var& value,
                           Var* exception);

  
  
  virtual void RemoveProperty(const Var& name,
                              Var* exception);

  

  
  
  
  
  virtual Var Call(const Var& method_name,
                   const std::vector<Var>& args,
                   Var* exception);

  
  
  virtual Var Construct(const std::vector<Var>& args,
                        Var* exception);

 private:
  friend class ::pp::Var;
  friend class ::pp::VarPrivate;
  static const PPP_Class_Deprecated* GetClass();

  
  ScriptableObject(const ScriptableObject& other);
  ScriptableObject& operator=(const ScriptableObject& other);
};

}  

}  

#endif  

