// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SHARED_MEMORY_H_
#define BASE_SHARED_MEMORY_H_
#pragma once

#include "build/build_config.h"

#if defined(OS_POSIX)
#include <sys/types.h>
#include <semaphore.h>
#include "base/file_descriptor_posix.h"
#endif
#include <string>

#include "base/base_api.h"
#include "base/basictypes.h"
#include "base/process.h"

class FilePath;

namespace base {

#if defined(OS_WIN)
typedef HANDLE SharedMemoryHandle;
typedef HANDLE SharedMemoryLock;
#elif defined(OS_POSIX)
typedef FileDescriptor SharedMemoryHandle;
typedef ino_t SharedMemoryId;
#endif

class BASE_API SharedMemory {
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

  
  
  bool CreateAndMapAnonymous(uint32 size);

  
  
  bool CreateAnonymous(uint32 size);

  
  
  
  
  
  
  bool CreateNamed(const std::string& name, bool open_existing, uint32 size);

  
  
  bool Delete(const std::string& name);

  
  
  
  bool Open(const std::string& name, bool read_only);

  
  
  
  bool Map(uint32 bytes);

  
  
  
  bool Unmap();

  
  
  
  
  
  
  
  
  uint32 created_size() const { return created_size_; }

  
  
  void *memory() const { return memory_; }

  
  
  
  SharedMemoryHandle handle() const;

#if defined(OS_POSIX)
  
  
  
  
  SharedMemoryId id() const { return inode_; }
#endif

  
  
  void Close();

  
  
  
  
  
  
  bool ShareToProcess(ProcessHandle process,
                      SharedMemoryHandle* new_handle) {
    return ShareToProcessCommon(process, new_handle, false);
  }

  
  
  
  
  
  
  bool GiveToProcess(ProcessHandle process,
                     SharedMemoryHandle* new_handle) {
    return ShareToProcessCommon(process, new_handle, true);
  }

  
  
  
  
  
  
  
  
  
  void Lock();

#if defined(OS_WIN)
  
  
  
  
  bool Lock(uint32 timeout_ms, SECURITY_ATTRIBUTES* sec_attr);
#endif

  
  void Unlock();

 private:
#if defined(OS_POSIX)
  bool PrepareMapFile(FILE *fp);
  bool FilePathForMemoryName(const std::string& mem_name, FilePath* path);
  void LockOrUnlockCommon(int function);
#endif
  bool ShareToProcessCommon(ProcessHandle process,
                            SharedMemoryHandle* new_handle,
                            bool close_self);

#if defined(OS_WIN)
  std::wstring       name_;
  HANDLE             mapped_file_;
#elif defined(OS_POSIX)
  int                mapped_file_;
  uint32             mapped_size_;
  ino_t              inode_;
#endif
  void*              memory_;
  bool               read_only_;
  uint32             created_size_;
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
