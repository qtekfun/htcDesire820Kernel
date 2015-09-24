// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_SERIALIZED_HANDLES_H_
#define PPAPI_PROXY_SERIALIZED_HANDLES_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/memory/shared_memory.h"
#include "build/build_config.h"
#include "ipc/ipc_platform_file.h"
#include "ppapi/proxy/ppapi_proxy_export.h"

class Pickle;

namespace ppapi {
namespace proxy {

class PPAPI_PROXY_EXPORT SerializedHandle {
 public:
  enum Type { INVALID, SHARED_MEMORY, SOCKET, CHANNEL_HANDLE, FILE };
  struct Header {
    Header() : type(INVALID), size(0), open_flag(0) {}
    Header(Type type_arg, uint32 size_arg, int32 open_flag_arg)
        : type(type_arg), size(size_arg), open_flag(open_flag_arg) {
    }
    Type type;
    uint32 size;
    int32 open_flag;
  };

  SerializedHandle();
  
  explicit SerializedHandle(Type type);

  
  SerializedHandle(const base::SharedMemoryHandle& handle, uint32 size);

  
  SerializedHandle(const Type type,
                   const IPC::PlatformFileForTransit& descriptor);

  Type type() const { return type_; }
  bool is_shmem() const { return type_ == SHARED_MEMORY; }
  bool is_socket() const { return type_ == SOCKET; }
  bool is_channel_handle() const { return type_ == CHANNEL_HANDLE; }
  bool is_file() const { return type_ == FILE; }
  const base::SharedMemoryHandle& shmem() const {
    DCHECK(is_shmem());
    return shm_handle_;
  }
  uint32 size() const {
    DCHECK(is_shmem());
    return size_;
  }
  const IPC::PlatformFileForTransit& descriptor() const {
    DCHECK(is_socket() || is_channel_handle() || is_file());
    return descriptor_;
  }
  int32 open_flag() const {
    return open_flag_;
  }
  void set_shmem(const base::SharedMemoryHandle& handle, uint32 size) {
    type_ = SHARED_MEMORY;
    shm_handle_ = handle;
    size_ = size;

    descriptor_ = IPC::InvalidPlatformFileForTransit();
  }
  void set_socket(const IPC::PlatformFileForTransit& socket) {
    type_ = SOCKET;
    descriptor_ = socket;

    shm_handle_ = base::SharedMemory::NULLHandle();
    size_ = 0;
  }
  void set_channel_handle(const IPC::PlatformFileForTransit& descriptor) {
    type_ = CHANNEL_HANDLE;

    descriptor_ = descriptor;
    shm_handle_ = base::SharedMemory::NULLHandle();
    size_ = 0;
  }
  void set_file_handle(const IPC::PlatformFileForTransit& descriptor,
                       int32 open_flag) {
    type_ = FILE;

    descriptor_ = descriptor;
    shm_handle_ = base::SharedMemory::NULLHandle();
    size_ = 0;
    open_flag_ = open_flag;
  }
  void set_null_shmem() {
    set_shmem(base::SharedMemory::NULLHandle(), 0);
  }
  void set_null_socket() {
    set_socket(IPC::InvalidPlatformFileForTransit());
  }
  void set_null_channel_handle() {
    set_channel_handle(IPC::InvalidPlatformFileForTransit());
  }
  void set_null_file_handle() {
    set_file_handle(IPC::InvalidPlatformFileForTransit(), 0);
  }
  bool IsHandleValid() const;

  Header header() const {
    return Header(type_, size_, open_flag_);
  }

  
  void Close();

  
  
  
  static bool WriteHeader(const Header& hdr, Pickle* pickle);
  static bool ReadHeader(PickleIterator* iter, Header* hdr);

 private:
  
  Type type_;

  
  
  
  
  
  base::SharedMemoryHandle shm_handle_;
  uint32 size_;

  
  IPC::PlatformFileForTransit descriptor_;

  
  int32 open_flag_;
};

}  
}  

#endif  
