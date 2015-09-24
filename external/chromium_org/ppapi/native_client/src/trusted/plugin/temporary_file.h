// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_TEMPORARY_FILE_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_TEMPORARY_FILE_H_

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/nacl_string.h"
#include "native_client/src/trusted/desc/nacl_desc_wrapper.h"

#include "ppapi/c/private/pp_file_handle.h"
#include "ppapi/cpp/completion_callback.h"

namespace plugin {

class Plugin;

//     written by: llc     (passed in explicitly through SRPC)
//     written by: ld      (passed in explicitly through SRPC)

class TempFile {
 public:
  
  explicit TempFile(Plugin* plugin);
  ~TempFile();

  
  
  
  void Open(const pp::CompletionCallback& cb, bool writeable);
  
  bool Reset();

  
  
  nacl::DescWrapper* write_wrapper() { return write_wrapper_.get(); }
  nacl::DescWrapper* read_wrapper() { return read_wrapper_.get(); }
  nacl::DescWrapper* release_read_wrapper() {
    return read_wrapper_.release();
  }

  PP_FileHandle* existing_handle() { return &existing_handle_; }

  
  const nacl::string identifier() const {
    return nacl::string(reinterpret_cast<const char*>(identifier_));
  }

 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(TempFile);

  Plugin* plugin_;
  nacl::scoped_ptr<nacl::DescWrapper> read_wrapper_;
  nacl::scoped_ptr<nacl::DescWrapper> write_wrapper_;
  PP_FileHandle existing_handle_;

  
  
  
  
  uint8_t identifier_[16];
  
  static uint32_t next_identifier;
};

}  

#endif  
