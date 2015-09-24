// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILE_UTIL_PROXY_H_
#define BASE_FILE_UTIL_PROXY_H_

#include <vector>

#include "base/base_api.h"
#include "base/callback.h"
#include "base/file_path.h"
#include "base/file_util.h"
#include "base/memory/ref_counted.h"
#include "base/platform_file.h"
#include "base/tracked_objects.h"

namespace base {

class MessageLoopProxy;
class Time;

class BASE_API FileUtilProxy {
 public:
  
  struct Entry {
    FilePath::StringType name;
    bool is_directory;
  };

  
  
  
  typedef Callback1<PlatformFileError >::Type StatusCallback;

  typedef Callback3<PlatformFileError ,
                    PassPlatformFile,
                    bool >::Type CreateOrOpenCallback;
  typedef Callback3<PlatformFileError ,
                    PassPlatformFile,
                    FilePath>::Type CreateTemporaryCallback;
  typedef Callback2<PlatformFileError ,
                    bool >::Type EnsureFileExistsCallback;
  typedef Callback2<PlatformFileError ,
                    const PlatformFileInfo& 
                    >::Type GetFileInfoCallback;
  typedef Callback2<PlatformFileError ,
                    const std::vector<Entry>&>::Type ReadDirectoryCallback;
  typedef Callback3<PlatformFileError ,
                    const char* ,
                    int /* bytes read/written */>::Type ReadCallback;
  typedef Callback2<PlatformFileError ,
                    int /* bytes written */>::Type WriteCallback;

  
  
  
  
  
  static bool CreateOrOpen(scoped_refptr<MessageLoopProxy> message_loop_proxy,
                           const FilePath& file_path,
                           int file_flags,
                           CreateOrOpenCallback* callback);

  
  
  static bool CreateTemporary(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      CreateTemporaryCallback* callback);

  
  static bool Close(scoped_refptr<MessageLoopProxy> message_loop_proxy,
                    PlatformFile,
                    StatusCallback* callback);

  
  
  
  
  
  
  
  
  
  static bool EnsureFileExists(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      const FilePath& file_path,
      EnsureFileExistsCallback* callback);

  
  
  static bool GetFileInfo(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      const FilePath& file_path,
      GetFileInfoCallback* callback);

  static bool GetFileInfoFromPlatformFile(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      PlatformFile file,
      GetFileInfoCallback* callback);

  static bool ReadDirectory(scoped_refptr<MessageLoopProxy> message_loop_proxy,
                            const FilePath& file_path,
                            ReadDirectoryCallback* callback);

  
  
  static bool CreateDirectory(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      const FilePath& file_path,
      bool exclusive,
      bool recursive,
      StatusCallback* callback);

  
  
  
  
  
  
  
  
  static bool Copy(scoped_refptr<MessageLoopProxy> message_loop_proxy,
                   const FilePath& src_file_path,
                   const FilePath& dest_file_path,
                   StatusCallback* callback);

  
  
  static bool Move(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      const FilePath& src_file_path,
      const FilePath& dest_file_path,
      StatusCallback* callback);

  
  
  static bool Delete(scoped_refptr<MessageLoopProxy> message_loop_proxy,
                     const FilePath& file_path,
                     bool recursive,
                     StatusCallback* callback);

  
  static bool RecursiveDelete(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      const FilePath& file_path,
      StatusCallback* callback);

  
  
  static bool Read(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      PlatformFile file,
      int64 offset,
      int bytes_to_read,
      ReadCallback* callback);

  
  
  
  static bool Write(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      PlatformFile file,
      int64 offset,
      const char* buffer,
      int bytes_to_write,
      WriteCallback* callback);

  
  static bool Touch(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      PlatformFile file,
      const Time& last_access_time,
      const Time& last_modified_time,
      StatusCallback* callback);

  
  static bool Touch(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      const FilePath& file_path,
      const Time& last_access_time,
      const Time& last_modified_time,
      StatusCallback* callback);

  
  
  
  static bool Truncate(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      PlatformFile file,
      int64 length,
      StatusCallback* callback);

  
  
  
  static bool Truncate(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      const FilePath& path,
      int64 length,
      StatusCallback* callback);

  
  static bool Flush(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      PlatformFile file,
      StatusCallback* callback);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(FileUtilProxy);
};

}  

#endif  
