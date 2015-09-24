// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_PRIVATE_VAR_PRIVATE_H_
#define PPAPI_CPP_PRIVATE_VAR_PRIVATE_H_

#include "ppapi/cpp/var.h"

namespace pp {

class InstanceHandle;

namespace deprecated {
class ScriptableObject;
}

class VarPrivate : public Var {
 public:
  VarPrivate() : Var() {}
  VarPrivate(Null) : Var(Null()) {}
  VarPrivate(bool b) : Var(b) {}
  VarPrivate(int32_t i) : Var(i) {}
  VarPrivate(double d) : Var(d) {}
  VarPrivate(const char* utf8_str) : Var(utf8_str) {}
  VarPrivate(const std::string& utf8_str) : Var(utf8_str) {}
  VarPrivate(PassRef, PP_Var var) : Var(PassRef(), var) {}
  VarPrivate(DontManage, PP_Var var) : Var(DontManage(), var) {}
  VarPrivate(const InstanceHandle& instance,
             deprecated::ScriptableObject* object);
  VarPrivate(const Var& other) : Var(other) {}

  virtual ~VarPrivate() {}

  
  
  
  deprecated::ScriptableObject* AsScriptableObject() const;

  bool HasProperty(const Var& name, Var* exception = NULL) const;
  bool HasMethod(const Var& name, Var* exception = NULL) const;
  VarPrivate GetProperty(const Var& name, Var* exception = NULL) const;
  void GetAllPropertyNames(std::vector<Var>* properties,
                           Var* exception = NULL) const;
  void SetProperty(const Var& name, const Var& value, Var* exception = NULL);
  void RemoveProperty(const Var& name, Var* exception = NULL);
  VarPrivate Call(const Var& method_name, uint32_t argc, Var* argv,
           Var* exception = NULL);
  VarPrivate Construct(uint32_t argc, Var* argv, Var* exception = NULL) const;

  
  VarPrivate Call(const Var& method_name, Var* exception = NULL);
  VarPrivate Call(const Var& method_name, const Var& arg1,
                  Var* exception = NULL);
  VarPrivate Call(const Var& method_name, const Var& arg1, const Var& arg2,
                  Var* exception = NULL);
  VarPrivate Call(const Var& method_name, const Var& arg1, const Var& arg2,
                  const Var& arg3, Var* exception = NULL);
  VarPrivate Call(const Var& method_name, const Var& arg1, const Var& arg2,
                  const Var& arg3, const Var& arg4, Var* exception = NULL);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  class OutException {
   public:
    OutException(Var* v)
        : output_(v),
          originally_had_exception_(v && !v->is_undefined()) {
      if (output_) {
        temp_ = output_->pp_var();
      } else {
        temp_.padding = 0;
        temp_.type = PP_VARTYPE_UNDEFINED;
      }
    }
    ~OutException() {
      if (output_ && !originally_had_exception_)
        *output_ = Var(PassRef(), temp_);
    }

    PP_Var* get() {
      if (output_)
        return &temp_;
      return NULL;
    }

   private:
    Var* output_;
    bool originally_had_exception_;
    PP_Var temp_;
  };

 private:
  
  
  
  VarPrivate(void* non_scriptable_object_pointer);
};

}  

#endif  
