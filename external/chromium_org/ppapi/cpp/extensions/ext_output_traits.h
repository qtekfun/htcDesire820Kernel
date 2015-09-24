// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_EXTENSIONS_OUTPUT_TRAITS_H_
#define PPAPI_CPP_EXTENSIONS_OUTPUT_TRAITS_H_

#include <vector>

#include "ppapi/c/pp_var.h"
#include "ppapi/cpp/extensions/from_var_converter.h"
#include "ppapi/cpp/logging.h"
#include "ppapi/cpp/pass_ref.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/var_array.h"

namespace pp {
namespace ext {
namespace internal {

template <class T>
class VarOutputAdapterWithStorage {
 public:
  VarOutputAdapterWithStorage() : pp_var_(PP_MakeUndefined()) {
  }

  ~VarOutputAdapterWithStorage() {
    PP_DCHECK(pp_var_.type == PP_VARTYPE_UNDEFINED);
  }

  PP_Var& pp_var() { return pp_var_; }

  T& output() {
    Var auto_release(PASS_REF, pp_var_);
    converter_.Set(pp_var_);
    pp_var_ = PP_MakeUndefined();
    return converter_.value();
  }

 private:
  PP_Var pp_var_;
  FromVarConverter<T> converter_;

  
  VarOutputAdapterWithStorage(const VarOutputAdapterWithStorage<T>&);
  VarOutputAdapterWithStorage<T>& operator=(
      const VarOutputAdapterWithStorage<T>&);
};

template <class T>
struct ExtCallbackOutputTraits {
  typedef PP_Var* APIArgType;
  typedef VarOutputAdapterWithStorage<T> StorageType;

  static inline APIArgType StorageToAPIArg(StorageType& t) {
    return &t.pp_var();
  }

  
  
  static inline T& StorageToPluginArg(StorageType& t) {
    return t.output();
  }

  static inline void Initialize(StorageType* ) {}
};

template <class T>
class ArrayVarOutputAdapterWithStorage {
 public:
  ArrayVarOutputAdapterWithStorage() : pp_var_(PP_MakeUndefined()) {
  }

  ~ArrayVarOutputAdapterWithStorage() {
    PP_DCHECK(pp_var_.type == PP_VARTYPE_UNDEFINED);
  }

  PP_Var& pp_var() { return pp_var_; }

  std::vector<T>& output() {
    PP_DCHECK(output_storage_.empty());

    Var var(PASS_REF, pp_var_);
    pp_var_ = PP_MakeUndefined();
    if (var.is_array()) {
      VarArray array(var);

      uint32_t length = array.GetLength();
      output_storage_.reserve(length);
      for (uint32_t i = 0; i < length; ++i) {
        FromVarConverter<T> converter(array.Get(i).pp_var());
        output_storage_.push_back(converter.value());
      }
    }

    return output_storage_;
  }

 private:
  PP_Var pp_var_;
  std::vector<T> output_storage_;

  
  ArrayVarOutputAdapterWithStorage(const ArrayVarOutputAdapterWithStorage<T>&);
  ArrayVarOutputAdapterWithStorage<T>& operator=(
      const ArrayVarOutputAdapterWithStorage<T>&);
};

template <class T>
struct ExtCallbackOutputTraits< std::vector<T> > {
  typedef PP_Var* APIArgType;
  typedef ArrayVarOutputAdapterWithStorage<T> StorageType;

  static inline APIArgType StorageToAPIArg(StorageType& t) {
    return &t.pp_var();
  }

  
  
  static inline std::vector<T>& StorageToPluginArg(StorageType& t) {
    return t.output();
  }

  static inline void Initialize(StorageType* ) {}
};

}  
}  
}  

#endif  
