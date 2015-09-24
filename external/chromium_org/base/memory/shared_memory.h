// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MEMORY_SHARED_MEMORY_H_
#define BASE_MEMORY_SHARED_MEMORY_H_

#include "build/build_config.h"

#include <string>

#if defined(OS_POSIX)
#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>
#endif

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/process/process_handle.h"

#if defined(OS_POSIX)
#include "base/file_descriptor_posix.h"
#include "base/file_util.h"
#endif

namespace base {

class FilePath;

#if defined(OS_WIN)
typedef HANDLE SharedMemoryHandle;
typedef HANDLE SharedMemoryLock;
#elif defined(OS_POSIX)
typedef FileDescriptor SharedMemoryHandle;
typedef ino_t SharedMemoryId;
#endif

struct SharedMemoryCreateOptions {
  SharedMemoryCreateOptions() : name(NULL), size(0), open_existing(false),
                                executable(false) {}

  
  
  const std::string* name;

  
  
  size_t size;

  
  
  
  
  bool open_existing;

  
  bool executable;
};

class BASE_EXPORT SharedMemory {
 public:
  SharedMemory();

#if defined(OS_WIN)
  
  
  
  explicit SharedMemory(const std::wstring& name);
#endif

  
  
  
  
  
  
  
  SharedMemory(SharedMemoryHandle handle, bool read_only);

  
  
  
  SharedMemory(SharedMemoryHandle handle, bool read_only,
               ProcessHandle process);

  
  ~SharedMemory();

  
  
  static bool IsHandleValid(const SharedMemoryHandle& handle);

  
  static SharedMemoryHandle NULLHandle();

  
  static void CloseHandle(const SharedMemoryHandle& handle);

  
  static size_t GetHandleLimit();

  
  
  bool Create(const SharedMemoryCreateOptions& options);

  
  
  bool CreateAndMapAnonymous(size_t size);

  
  
  bool CreateAnonymous(size_t size) {
    SharedMemoryCreateOptions options;
    options.size = size;
    return Create(options);
  }

  
  
  
  
  
  
  bool CreateNamed(const std::string& name, bool open_existing, size_t size) {
    SharedMemoryCreateOptions options;
    options.name = &name;
    options.open_existing = open_existing;
    options.size = size;
    return Create(options);
  }

  
  
  bool Delete(const std::string& name);

  
  
  
  bool Open(const std::string& name, bool read_only);

  
  
  
  
  bool Map(size_t bytes) {
    return MapAt(0, bytes);
  }

  
  
  
  bool MapAt(off_t offset, size_t bytes);
  enum { MAP_MINIMUM_ALIGNMENT = 32 };

  
  
  
  bool Unmap();

  
  size_t requested_size() const { return requested_size_; }

  
  size_t mapped_size() const { return mapped_size_; }

  
  
  void *memory() const { return memory_; }

  
  
  
  SharedMemoryHandle handle() const;

#if defined(OS_POSIX) && !defined(OS_NACL)
  
  
  
  
  SharedMemoryId id() const { return inode_; }
#endif

  
  
  void Close();

  
  
  
  
  
  
  
  
  
  
  bool ShareReadOnlyToProcess(ProcessHandle process,
                              SharedMemoryHandle* new_handle) {
    return ShareToProcessCommon(process, new_handle, false, SHARE_READONLY);
  }

  
  
  
  
  
  
  bool GiveReadOnlyToProcess(ProcessHandle process,
                             SharedMemoryHandle* new_handle) {
    return ShareToProcessCommon(process, new_handle, true, SHARE_READONLY);
  }

  
  
  
  
  
  
  bool ShareToProcess(ProcessHandle process,
                      SharedMemoryHandle* new_handle) {
    return ShareToProcessCommon(process, new_handle, false, SHARE_CURRENT_MODE);
  }

  
  
  
  
  
  
  bool GiveToProcess(ProcessHandle process,
                     SharedMemoryHandle* new_handle) {
    return ShareToProcessCommon(process, new_handle, true, SHARE_CURRENT_MODE);
  }

  
  
  
  
  
  
  void Lock();

#if defined(OS_WIN)
  
  
  
  
  bool Lock(uint32 timeout_ms, SECURITY_ATTRIBUTES* sec_attr);
#endif

  
  void Unlock();

 private:
#if defined(OS_POSIX) && !defined(OS_NACL)
  bool PrepareMapFile(file_util::ScopedFILE fp, file_util::ScopedFD readonly);
  bool FilePathForMemoryName(const std::string& mem_name, FilePath* path);
  void LockOrUnlockCommon(int function);
#endif
  enum ShareMode {
    SHARE_READONLY,
    SHARE_CURRENT_MODE,
  };
  bool ShareToProcessCommon(ProcessHandle process,
                            SharedMemoryHandle* new_handle,
                            bool close_self,
                            ShareMode);

#if defined(OS_WIN)
  std::wstring       name_;
  HANDLE             mapped_file_;
#elif defined(OS_POSIX)
  int                mapped_file_;
  int                readonly_mapped_file_;
  ino_t              inode_;
#endif
  size_t             mapped_size_;
  void*              memory_;
  bool               read_only_;
  size_t             requested_size_;
#if !defined(OS_POSIX)
  SharedMemoryLock   lock_;
#endif

  DISALLOW_COPY_AND_ASSIGN(SharedMemory);
};

class SharedMemoryAutoLock {
 public:
  explicit SharedMemoryAutoLock(SharedMemory* shared_memory)
      : shared_memory_(shared_memory) {
    shared_memory_->Lock();
  }

  ~SharedMemoryAutoLock() {
    shared_memory_->Unlock();
  }

 private:
  SharedMemory* shared_memory_;
  DISALLOW_COPY_AND_ASSIGN(SharedMemoryAutoLock);
};

}  

#endif  
