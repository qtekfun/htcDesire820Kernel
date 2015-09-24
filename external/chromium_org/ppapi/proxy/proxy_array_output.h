// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PROXY_ARRAY_OUTPUT_H_
#define PPAPI_PROXY_PROXY_ARRAY_OUTPUT_H_

#include <vector>

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "ppapi/c/pp_array_output.h"

namespace ppapi {
namespace proxy {

class ArrayOutputAdapterBase {
 public:
  ArrayOutputAdapterBase() {
    pp_array_output_.GetDataBuffer =
        &ArrayOutputAdapterBase::GetDataBufferThunk;
    pp_array_output_.user_data = this;
  }
  virtual ~ArrayOutputAdapterBase() {}

  const PP_ArrayOutput& pp_array_output() { return pp_array_output_; }

 protected:
  virtual void* GetDataBuffer(uint32_t element_count,
                              uint32_t element_size) = 0;

 private:
  static void* GetDataBufferThunk(void* user_data,
                                  uint32_t element_count,
                                  uint32_t element_size);

  PP_ArrayOutput pp_array_output_;

  
  
  ArrayOutputAdapterBase(const ArrayOutputAdapterBase&);
  ArrayOutputAdapterBase& operator=(const ArrayOutputAdapterBase&);
};

template<typename T>
class ArrayOutputAdapter : public ArrayOutputAdapterBase {
 public:
  ArrayOutputAdapter(std::vector<T>* output) : output_(output) {}

 protected:
  
  ArrayOutputAdapter() : output_(NULL) {}
  void set_output(std::vector<T>* output) { output_ = output; }

  
  virtual void* GetDataBuffer(uint32_t element_count, uint32_t element_size) {
    DCHECK(element_size == sizeof(T));
    if (element_count == 0 || element_size != sizeof(T))
      return NULL;
    output_->resize(element_count);
    return &(*output_)[0];
  }

 private:
  std::vector<T>* output_;
};

template<typename T>
class ArrayOutputAdapterWithStorage : public ArrayOutputAdapter<T> {
 public:
  ArrayOutputAdapterWithStorage() {
    
    
    this->set_output(&output_storage_);
  }

  std::vector<T>& output() { return output_storage_; }

 private:
  std::vector<T> output_storage_;
};

template<typename T>
class RefCountedArrayOutputAdapter
    : public ArrayOutputAdapterWithStorage<T>,
      public base::RefCounted<RefCountedArrayOutputAdapter<T> > {
 public:
  RefCountedArrayOutputAdapter()
      : ArrayOutputAdapterWithStorage<T>() {
  }
};

}  
}  

#endif  
