/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_FILE_IO_H_
#define PPAPI_C_PPB_FILE_IO_H_

#include "ppapi/c/pp_array_output.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_file_info.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"

#define PPB_FILEIO_INTERFACE_1_0 "PPB_FileIO;1.0"
#define PPB_FILEIO_INTERFACE_1_1 "PPB_FileIO;1.1"
#define PPB_FILEIO_INTERFACE PPB_FILEIO_INTERFACE_1_1



typedef enum {
  
  PP_FILEOPENFLAG_READ = 1 << 0,
  PP_FILEOPENFLAG_WRITE = 1 << 1,
  PP_FILEOPENFLAG_CREATE = 1 << 2,
  PP_FILEOPENFLAG_TRUNCATE = 1 << 3,
  PP_FILEOPENFLAG_EXCLUSIVE = 1 << 4,
  PP_FILEOPENFLAG_APPEND = 1 << 5
} PP_FileOpenFlags;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_FileOpenFlags, 4);

struct PPB_FileIO_1_1 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsFileIO)(PP_Resource resource);
  int32_t (*Open)(PP_Resource file_io,
                  PP_Resource file_ref,
                  int32_t open_flags,
                  struct PP_CompletionCallback callback);
  int32_t (*Query)(PP_Resource file_io,
                   struct PP_FileInfo* info,
                   struct PP_CompletionCallback callback);
  int32_t (*Touch)(PP_Resource file_io,
                   PP_Time last_access_time,
                   PP_Time last_modified_time,
                   struct PP_CompletionCallback callback);
  int32_t (*Read)(PP_Resource file_io,
                  int64_t offset,
                  char* buffer,
                  int32_t bytes_to_read,
                  struct PP_CompletionCallback callback);
  /**
   * Write() writes to an offset in the file.  This function might perform a
   * partial write. The FileIO object must have been opened with write access.
   *
   * @param[in] file_io A <code>PP_Resource</code> corresponding to a file
   * FileIO.
   * @param[in] offset The offset into the file.
   * @param[in] buffer The buffer to hold the specified number of bytes read.
   * @param[in] bytes_to_write The number of bytes to write to
   * <code>offset</code>.
   * @param[in] callback A <code>PP_CompletionCallback</code> to be called upon
   * completion of Write().
   *
   * @return The number of bytes written or an error code from
   * <code>pp_errors.h</code>. If the return value is 0, then end-of-file was
   * reached. It is valid to call Write() multiple times with a completion
   * callback to queue up parallel writes to the file, but pending writes
   * cannot be interleaved with other operations.
   */
  int32_t (*Write)(PP_Resource file_io,
                   int64_t offset,
                   const char* buffer,
                   int32_t bytes_to_write,
                   struct PP_CompletionCallback callback);
  int32_t (*SetLength)(PP_Resource file_io,
                       int64_t length,
                       struct PP_CompletionCallback callback);
  int32_t (*Flush)(PP_Resource file_io, struct PP_CompletionCallback callback);
  void (*Close)(PP_Resource file_io);
  int32_t (*ReadToArray)(PP_Resource file_io,
                         int64_t offset,
                         int32_t max_read_length,
                         struct PP_ArrayOutput* output,
                         struct PP_CompletionCallback callback);
};

typedef struct PPB_FileIO_1_1 PPB_FileIO;

struct PPB_FileIO_1_0 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsFileIO)(PP_Resource resource);
  int32_t (*Open)(PP_Resource file_io,
                  PP_Resource file_ref,
                  int32_t open_flags,
                  struct PP_CompletionCallback callback);
  int32_t (*Query)(PP_Resource file_io,
                   struct PP_FileInfo* info,
                   struct PP_CompletionCallback callback);
  int32_t (*Touch)(PP_Resource file_io,
                   PP_Time last_access_time,
                   PP_Time last_modified_time,
                   struct PP_CompletionCallback callback);
  int32_t (*Read)(PP_Resource file_io,
                  int64_t offset,
                  char* buffer,
                  int32_t bytes_to_read,
                  struct PP_CompletionCallback callback);
  int32_t (*Write)(PP_Resource file_io,
                   int64_t offset,
                   const char* buffer,
                   int32_t bytes_to_write,
                   struct PP_CompletionCallback callback);
  int32_t (*SetLength)(PP_Resource file_io,
                       int64_t length,
                       struct PP_CompletionCallback callback);
  int32_t (*Flush)(PP_Resource file_io, struct PP_CompletionCallback callback);
  void (*Close)(PP_Resource file_io);
};

#endif  

