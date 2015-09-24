// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_ARRAY_WRITER_H_
#define PPAPI_SHARED_IMPL_ARRAY_WRITER_H_

#include <string.h>

#include <vector>

#include "base/memory/ref_counted.h"
#include "ppapi/c/pp_array_output.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

class Resource;
class Var;

class PPAPI_SHARED_EXPORT ArrayWriter {
 public:
  ArrayWriter();  
  ArrayWriter(const PP_ArrayOutput& output);
  ~ArrayWriter();

  bool is_valid() const { return !!pp_array_output_.GetDataBuffer; }
  bool is_null() const { return !is_valid(); }

  void set_pp_array_output(const PP_ArrayOutput& output) {
    pp_array_output_ = output;
  }

  
  void Reset();

  
  
  
  
  
  
  
  
  
  
  
  template <typename T>
  bool StoreArray(const T* input, uint32_t count) {
    
    void* dest = pp_array_output_.GetDataBuffer(
        pp_array_output_.user_data,
        count,
        sizeof(T));

    
    
    Reset();

    if (count == 0)
      return true;  
    if (!dest)
      return false;

    if (input)
      memcpy(dest, input, sizeof(T) * count);
    return true;
  }

  
  
  template<typename T>
  bool StoreVector(const std::vector<T>& input) {
    return StoreArray(input.size() ? &input[0] : NULL, input.size());
  }

  
  
  
  
  
  
  
  
  
  
  
  bool StoreResourceVector(const std::vector< scoped_refptr<Resource> >& input);

  
  
  bool StoreResourceVector(const std::vector<PP_Resource>& input);

  
  
  
  
  
  
  
  bool StoreVarVector(const std::vector< scoped_refptr<Var> >& input);

  
  
  bool StoreVarVector(const std::vector<PP_Var>& input);

 private:
  PP_ArrayOutput pp_array_output_;

  DISALLOW_COPY_AND_ASSIGN(ArrayWriter);
};

}  

#endif  
