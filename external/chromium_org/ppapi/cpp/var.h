// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_VAR_H_
#define PPAPI_CPP_VAR_H_

#include <string>
#include <vector>

#include "ppapi/c/pp_var.h"
#include "ppapi/cpp/pass_ref.h"

namespace pp {

class Var {
 public:
  
  struct Null {};

  
  
  Var();

  
  Var(Null);

  
  
  
  Var(bool b);

  
  
  
  Var(int32_t i);

  
  
  
  Var(double d);

  
  Var(const char* utf8_str);  

  
  Var(const std::string& utf8_str);  

  
  
  
  
  
  Var(PassRef, const PP_Var& var) {
    var_ = var;
    is_managed_ = true;
  }

  
  explicit Var(const PP_Var& var);

  struct DontManage {};

  
  
  
  
  
  
  
  
  Var(DontManage, const PP_Var& var) {
    var_ = var;
    is_managed_ = false;
  }

  
  Var(const Var& other);

  
  virtual ~Var();

  
  
  
  
  
  virtual Var& operator=(const Var& other);

  
  
  
  
  
  
  
  bool operator==(const Var& other) const;

  
  
  
  bool is_undefined() const { return var_.type == PP_VARTYPE_UNDEFINED; }

  
  
  
  bool is_null() const { return var_.type == PP_VARTYPE_NULL; }

  
  
  
  bool is_bool() const { return var_.type == PP_VARTYPE_BOOL; }

  
  
  
  bool is_string() const { return var_.type == PP_VARTYPE_STRING; }

  
  
  
  bool is_object() const { return var_.type == PP_VARTYPE_OBJECT; }

  
  
  
  bool is_array() const { return var_.type == PP_VARTYPE_ARRAY; }

  
  
  
  bool is_dictionary() const { return var_.type == PP_VARTYPE_DICTIONARY; }

  
  
  
  bool is_resource() const { return var_.type == PP_VARTYPE_RESOURCE; }

  
  
  
  
  
  
  
  
  bool is_int() const { return var_.type == PP_VARTYPE_INT32; }

  
  
  
  
  
  
  
  
  bool is_double() const { return var_.type == PP_VARTYPE_DOUBLE; }

  
  
  
  
  bool is_number() const {
    return var_.type == PP_VARTYPE_INT32 ||
           var_.type == PP_VARTYPE_DOUBLE;
  }

  
  bool is_array_buffer() const { return var_.type == PP_VARTYPE_ARRAY_BUFFER; }

  
  
  
  
  
  bool AsBool() const;

  
  
  
  
  
  
  
  
  
  
  
  int32_t AsInt() const;

  
  
  
  
  
  
  
  
  
  
  
  double AsDouble() const;

  
  
  
  
  std::string AsString() const;

  
  
  
  
  const PP_Var& pp_var() const {
    return var_;
  }

  
  
  
  
  
  
  
  PP_Var Detach() {
    PP_Var ret = var_;
    var_ = PP_MakeUndefined();
    is_managed_ = true;
    return ret;
  }

  
  
  
  
  
  
  std::string DebugString() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  class OutException {
   public:
    
    OutException(Var* v)
        : output_(v),
          originally_had_exception_(v && !v->is_undefined()) {
      if (output_) {
        temp_ = output_->var_;
      } else {
        temp_.padding = 0;
        temp_.type = PP_VARTYPE_UNDEFINED;
      }
    }

    
    ~OutException() {
      if (output_ && !originally_had_exception_)
        *output_ = Var(PASS_REF, temp_);
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

 protected:
  PP_Var var_;

  
  
  bool is_managed_;

 private:
  
  
  
  Var(void* non_scriptable_object_pointer);
};

}  

#endif  
