// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_PRIVATE_PASS_FILE_HANDLE_H_
#define PPAPI_CPP_PRIVATE_PASS_FILE_HANDLE_H_

#include <string.h>

#include "ppapi/c/private/pp_file_handle.h"
#include "ppapi/cpp/output_traits.h"

namespace pp {

class PassFileHandle {
 public:
  PassFileHandle();
  
  explicit PassFileHandle(PP_FileHandle handle);
  
  PassFileHandle(PassFileHandle& handle);
  ~PassFileHandle();

  
  PP_FileHandle Release();

 private:
  
  
  struct PassFileHandleRef {
    PP_FileHandle handle;
    explicit PassFileHandleRef(PP_FileHandle h)
        : handle(h) {
    }
  };

 public:
  PassFileHandle(PassFileHandleRef ref)
      : handle_(ref.handle) {
  }

  operator PassFileHandleRef() {
    return PassFileHandleRef(Release());
  }

 private:
  void operator=(const PassFileHandle&);

  void Close();

  PP_FileHandle handle_;
};

namespace internal {

template<>
struct CallbackOutputTraits<PassFileHandle> {
  typedef PP_FileHandle* APIArgType;
  typedef PP_FileHandle StorageType;

  static inline APIArgType StorageToAPIArg(StorageType& t) {
    return &t;
  }

  static inline PassFileHandle StorageToPluginArg(StorageType& t) {
    return PassFileHandle(t);
  }

  static inline void Initialize(StorageType* t) {
    memset(t, 0, sizeof(*t));
  }
};

}  
}  

#endif  
