// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_RESOURCE_ARRAY_DEV_H_
#define PPAPI_CPP_DEV_RESOURCE_ARRAY_DEV_H_

#include "ppapi/c/pp_array_output.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class InstanceHandle;

class ResourceArray_Dev : public Resource {
 public:
  
  
  struct ArrayOutputCallbackData {
    ArrayOutputCallbackData(const PP_ArrayOutput& array_output,
                            const PP_CompletionCallback& callback)
        : resource_array_output(0),
          output(array_output),
          original_callback(callback) {
    }

    PP_Resource resource_array_output;
    PP_ArrayOutput output;
    PP_CompletionCallback original_callback;
  };

  ResourceArray_Dev();

  ResourceArray_Dev(PassRef, PP_Resource resource);

  ResourceArray_Dev(const ResourceArray_Dev& other);

  ResourceArray_Dev(const InstanceHandle& instance,
                    const PP_Resource elements[],
                    uint32_t size);

  virtual ~ResourceArray_Dev();

  ResourceArray_Dev& operator=(const ResourceArray_Dev& other);

  uint32_t size() const;

  PP_Resource operator[](uint32_t index) const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static void ArrayOutputCallbackConverter(void* user_data, int32_t result);
};

}  

#endif  
