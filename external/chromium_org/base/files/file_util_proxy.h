// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILES_FILE_UTIL_PROXY_H_
#define BASE_FILES_FILE_UTIL_PROXY_H_

#include "base/base_export.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/platform_file.h"

namespace tracked_objects {
class Location;
};

namespace base {

class TaskRunner;
class Time;

class BASE_EXPORT FileUtilProxy {
 public:
  
  
  
  typedef Callback<void(PlatformFileError)> StatusCallback;

  typedef Callback<void(PlatformFileError,
                        PassPlatformFile,
                        bool )> CreateOrOpenCallback;
  typedef Callback<void(PlatformFileError,
                        PassPlatformFile,
                        const FilePath&)> CreateTemporaryCallback;
  typedef Callback<void(PlatformFileError,
                        const PlatformFileInfo&)> GetFileInfoCallback;
  typedef Callback<void(PlatformFileError,
                        const char* ,
                        int )> ReadCallback;
  typedef Callback<void(PlatformFileError,
                        int /* bytes written */)> WriteCallback;

  typedef Callback<PlatformFileError(PlatformFile*, bool*)> CreateOrOpenTask;
  typedef Callback<PlatformFileError(PlatformFile)> CloseTask;
  typedef Callback<PlatformFileError(void)> FileTask;

  
  
  
  
  
  
  static bool CreateOrOpen(TaskRunner* task_runner,
                           const FilePath& file_path,
                           int file_flags,
                           const CreateOrOpenCallback& callback);

  
  
  
  
  
  
  
  
  
  
  static bool CreateTemporary(
      TaskRunner* task_runner,
      int additional_file_flags,
      const CreateTemporaryCallback& callback);

  
  
  static bool Close(TaskRunner* task_runner,
                    PlatformFile,
                    const StatusCallback& callback);

  
  
  
  static bool GetFileInfo(
      TaskRunner* task_runner,
      const FilePath& file_path,
      const GetFileInfoCallback& callback);

  
  
  static bool GetFileInfoFromPlatformFile(
      TaskRunner* task_runner,
      PlatformFile file,
      const GetFileInfoCallback& callback);

  
  
  
  static bool DeleteFile(TaskRunner* task_runner,
                         const FilePath& file_path,
                         bool recursive,
                         const StatusCallback& callback);

  
  
  
  
  
  static bool Read(
      TaskRunner* task_runner,
      PlatformFile file,
      int64 offset,
      int bytes_to_read,
      const ReadCallback& callback);

  
  
  
  
  
  
  
  static bool Write(
      TaskRunner* task_runner,
      PlatformFile file,
      int64 offset,
      const char* buffer,
      int bytes_to_write,
      const WriteCallback& callback);

  
  
  static bool Touch(
      TaskRunner* task_runner,
      PlatformFile file,
      const Time& last_access_time,
      const Time& last_modified_time,
      const StatusCallback& callback);

  
  
  static bool Touch(
      TaskRunner* task_runner,
      const FilePath& file_path,
      const Time& last_access_time,
      const Time& last_modified_time,
      const StatusCallback& callback);

  
  
  
  
  static bool Truncate(
      TaskRunner* task_runner,
      PlatformFile file,
      int64 length,
      const StatusCallback& callback);

  
  
  
  
  static bool Truncate(
      TaskRunner* task_runner,
      const FilePath& path,
      int64 length,
      const StatusCallback& callback);

  
  
  static bool Flush(
      TaskRunner* task_runner,
      PlatformFile file,
      const StatusCallback& callback);

  
  
  static bool RelayCreateOrOpen(
      TaskRunner* task_runner,
      const CreateOrOpenTask& open_task,
      const CloseTask& close_task,
      const CreateOrOpenCallback& callback);
  static bool RelayClose(
      TaskRunner* task_runner,
      const CloseTask& close_task,
      PlatformFile,
      const StatusCallback& callback);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(FileUtilProxy);
};

}  

#endif  
