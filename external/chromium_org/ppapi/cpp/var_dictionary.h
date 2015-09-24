// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_VAR_DICTIONARY_H_
#define PPAPI_CPP_VAR_DICTIONARY_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/var_array.h"


namespace pp {

class VarDictionary : public Var {
 public:
  
  VarDictionary();

  
  
  
  
  
  explicit VarDictionary(const Var& var);

  
  
  
  
  explicit VarDictionary(const PP_Var& var);

  
  VarDictionary(const VarDictionary& other);

  virtual ~VarDictionary();

  
  VarDictionary& operator=(const VarDictionary& other);

  
  
  
  
  
  
  
  
  virtual Var& operator=(const Var& other);

  
  
  
  
  
  
  
  Var Get(const Var& key) const;

  
  
  
  
  
  
  
  
  bool Set(const Var& key, const Var& value);

  
  
  
  void Delete(const Var& key);

  
  
  
  
  
  bool HasKey(const Var& key) const;

  
  
  
  
  VarArray GetKeys() const;
};

}  

#endif  
