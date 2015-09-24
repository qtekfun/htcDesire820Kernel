// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRARIES_NACL_IO_KERNEL_OBJECT_H_
#define LIBRARIES_NACL_IO_KERNEL_OBJECT_H_

#include <pthread.h>

#include <map>
#include <string>
#include <vector>

#include "nacl_io/error.h"
#include "nacl_io/kernel_handle.h"
#include "nacl_io/mount.h"
#include "nacl_io/mount_node.h"
#include "nacl_io/path.h"

#include "sdk_util/macros.h"
#include "sdk_util/simple_lock.h"

namespace nacl_io {

class KernelObject {
 public:
  struct Descriptor_t {
    Descriptor_t() : flags(0) {}
    explicit Descriptor_t(const ScopedKernelHandle& h) : handle(h), flags(0) {}

    ScopedKernelHandle handle;
    int flags;
  };
  typedef std::vector<Descriptor_t> HandleMap_t;
  typedef std::map<std::string, ScopedMount> MountMap_t;

  KernelObject();
  virtual ~KernelObject();

  
  Error AttachMountAtPath(const ScopedMount& mnt, const std::string& path);

  
  Error DetachMountAtPath(const std::string& path);

  
  
  
  Error AcquireMountAndRelPath(const std::string& path,
                               ScopedMount* out_mount,
                               Path* rel_path);

  
  
  
  Error AcquireMountAndNode(const std::string& path,
                            int oflags,
                            ScopedMount* out_mount,
                            ScopedMountNode* out_node);

  
  Error GetFDFlags(int fd, int* out_flags);
  
  Error SetFDFlags(int fd, int flags);

  
  
  Error AcquireHandle(int fd, ScopedKernelHandle* out_handle);

  
  
  
  int AllocateFD(const ScopedKernelHandle& handle);

  
  void FreeAndReassignFD(int fd, const ScopedKernelHandle& handle);
  void FreeFD(int fd);

  
  std::string GetCWD();
  Error SetCWD(const std::string& path);

  
  Path GetAbsParts(const std::string& path);

private:
  std::string cwd_;
  std::vector<int> free_fds_;
  HandleMap_t handle_map_;
  MountMap_t mounts_;

  
  sdk_util::SimpleLock handle_lock_;

  
  sdk_util::SimpleLock mount_lock_;

  
  sdk_util::SimpleLock cwd_lock_;

  DISALLOW_COPY_AND_ASSIGN(KernelObject);
};

}  

#endif  
