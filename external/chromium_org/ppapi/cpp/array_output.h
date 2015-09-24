// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_ARRAY_OUTPUT_H_
#define PPAPI_CPP_ARRAY_OUTPUT_H_

#include <vector>

#include "ppapi/c/pp_array_output.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/cpp/logging.h"
#include "ppapi/cpp/pass_ref.h"
#include "ppapi/cpp/var.h"

namespace pp {

template<typename ResourceObjectType>
inline void ConvertPPResourceArrayToObjects(
    PassRef,
    const std::vector<PP_Resource>& input,
    std::vector<ResourceObjectType>* output) {
  output->resize(0);
  output->reserve(input.size());
  for (size_t i = 0; i < input.size(); i++)
    output->push_back(ResourceObjectType(PASS_REF, input[i]));
}

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
    if (element_count == 0)
      return NULL;
    PP_DCHECK(element_size == sizeof(T));
    if (element_size != sizeof(T))
      return NULL;
    output_->resize(element_count);
    return &(*output_)[0];
  }

 private:
  std::vector<T>* output_;
};

template<typename T>
class ResourceArrayOutputAdapter : public ArrayOutputAdapterBase {
 public:
  explicit ResourceArrayOutputAdapter(std::vector<T>* output)
      : output_(output) {
    output_->resize(0);
  }
  virtual ~ResourceArrayOutputAdapter() {
    ConvertPPResourceArrayToObjects(PASS_REF, intermediate_output_, output_);
  }

 protected:
  
  ResourceArrayOutputAdapter() : output_(NULL) {}
  void set_output(T* output) { output_ = output; }

  
  virtual void* GetDataBuffer(uint32_t element_count,
                              uint32_t element_size) {
    if (element_count == 0)
      return NULL;
    PP_DCHECK(element_size == sizeof(PP_Resource));
    if (element_size != sizeof(PP_Resource))
      return NULL;
    intermediate_output_.resize(element_count);
    return &intermediate_output_[0];
  }

 private:
  std::vector<PP_Resource> intermediate_output_;
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

class VarArrayOutputAdapterWithStorage : public ArrayOutputAdapter<PP_Var> {
 public:
  VarArrayOutputAdapterWithStorage();
  virtual ~VarArrayOutputAdapterWithStorage();

  
  // written by the browser to pp::Var objects.
  
  
  
  std::vector<Var>& output();

 private:
  
  std::vector<PP_Var> temp_storage_;

  
  
  std::vector<Var> output_storage_;
};

template<typename T>
class ResourceArrayOutputAdapterWithStorage
    : public ArrayOutputAdapter<PP_Resource> {
 public:
  ResourceArrayOutputAdapterWithStorage() {
    set_output(&temp_storage_);
  }

  virtual ~ResourceArrayOutputAdapterWithStorage() {
    if (!temp_storage_.empty()) {
      
      output();
    }
  }

  
  // written by the browser to resource objects.
  
  
  
  std::vector<T>& output() {
    PP_DCHECK(output_storage_.empty());

    ConvertPPResourceArrayToObjects(PASS_REF, temp_storage_, &output_storage_);
    temp_storage_.clear();
    return output_storage_;
  }

 private:
  
  std::vector<PP_Resource> temp_storage_;

  
  
  std::vector<T> output_storage_;
};

}  

#endif  
