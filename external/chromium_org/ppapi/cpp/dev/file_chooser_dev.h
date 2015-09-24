// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_FILE_CHOOSER_DEV_H_
#define PPAPI_CPP_DEV_FILE_CHOOSER_DEV_H_

#include <vector>

#include "ppapi/c/dev/ppb_file_chooser_dev.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/file_ref.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class CompletionCallback;
class FileRef;
class InstanceHandle;
class Var;

class FileChooser_Dev : public Resource {
 public:
  
  FileChooser_Dev() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  FileChooser_Dev(const InstanceHandle& instance,
                  PP_FileChooserMode_Dev mode,
                  const Var& accept_types);

  FileChooser_Dev(const FileChooser_Dev& other);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int32_t Show(
      const CompletionCallbackWithOutput< std::vector<FileRef> >& callback);

 protected:
  
  struct ChooseCallbackData0_5 {
    PP_Resource file_chooser;
    PP_ArrayOutput output;
    PP_CompletionCallback original_callback;
  };

  
  
  
  
  
  
  static void CallbackConverter(void* user_data, int32_t result);
};

}  

#endif  
