// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_FILE_IO_H_
#define PPAPI_CPP_FILE_IO_H_

#include "ppapi/c/pp_time.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/resource.h"


struct PP_FileInfo;

namespace pp {

class FileRef;
class InstanceHandle;

class FileIO : public Resource {
 public:
  
  
  FileIO();

  
  
  
  
  
  explicit FileIO(const InstanceHandle& instance);

  
  
  
  FileIO(const FileIO& other);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Open(const FileRef& file_ref,
               int32_t open_flags,
               const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Query(PP_FileInfo* result_buf,
                const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  
  int32_t Touch(PP_Time last_access_time,
                PP_Time last_modified_time,
                const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Read(int64_t offset,
               char* buffer,
               int32_t bytes_to_read,
               const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Read(int32_t offset,
               int32_t max_read_length,
               const CompletionCallbackWithOutput< std::vector<char> >& cc);

  
  
  
  
  
  
  
  
  
  
  /// @return An The number of bytes written or an error code from
  
  
  
  int32_t Write(int64_t offset,
                const char* buffer,
                int32_t bytes_to_write,
                const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  
  int32_t SetLength(int64_t length,
                    const CompletionCallback& cc);

  
  
  
  
  
  
  
  int32_t Flush(const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  void Close();

 private:
  struct CallbackData1_0 {
    PP_ArrayOutput output;
    char* temp_buffer;
    PP_CompletionCallback original_callback;
  };

  
  
  
  
  
  
  static void CallbackConverter(void* user_data, int32_t result);
};

}  

#endif  
