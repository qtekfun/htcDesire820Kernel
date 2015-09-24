// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_FILE_UTILS_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_FILE_UTILS_H_

#include "native_client/src/include/nacl_string.h"
#include "native_client/src/include/portability_io.h"
#include "ppapi/c/pp_stdint.h"

namespace plugin {
namespace file_utils {

enum StatusCode {
  PLUGIN_FILE_SUCCESS = 0,
  PLUGIN_FILE_ERROR_MEM_ALLOC = 1,
  PLUGIN_FILE_ERROR_OPEN = 2,
  PLUGIN_FILE_ERROR_FILE_TOO_LARGE = 3,
  PLUGIN_FILE_ERROR_STAT = 4,
  PLUGIN_FILE_ERROR_READ = 5
};

StatusCode SlurpFile(int32_t fd,
                     nacl::string& out_buf,
                     size_t max_size_to_read = (1 << 20));

}  
}  

#endif  

