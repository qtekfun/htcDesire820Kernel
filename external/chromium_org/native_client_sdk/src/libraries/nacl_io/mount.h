// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRARIES_NACL_IO_MOUNT_H_
#define LIBRARIES_NACL_IO_MOUNT_H_

#include <map>
#include <string>

#include "nacl_io/error.h"
#include "nacl_io/inode_pool.h"
#include "nacl_io/mount_node.h"
#include "nacl_io/path.h"

#include "sdk_util/macros.h"
#include "sdk_util/ref_object.h"
#include "sdk_util/scoped_ref.h"

struct fuse_operations;

namespace nacl_io {

class Mount;
class MountNode;
class PepperInterface;

typedef sdk_util::ScopedRef<Mount> ScopedMount;
typedef std::map<std::string, std::string> StringMap_t;

struct MountInitArgs {
  MountInitArgs() : dev(0), ppapi(NULL), fuse_ops(NULL) {}
  explicit MountInitArgs(int dev) : dev(dev), ppapi(NULL), fuse_ops(NULL) {}

  
  int dev;
  StringMap_t string_map;
  PepperInterface* ppapi;
  fuse_operations* fuse_ops;
};

class Mount : public sdk_util::RefObject {
 protected:
  
  
  Mount();
  virtual ~Mount();

  
  
  virtual Error Init(const MountInitArgs& args);
  virtual void Destroy();

 public:
  PepperInterface* ppapi() { return ppapi_; }

  

  
  
  virtual Error Access(const Path& path, int a_mode) = 0;

  
  
  
  virtual Error Open(const Path& path,
                     int open_flags,
                     ScopedMountNode* out_node) = 0;

  
  
  
  virtual Error OpenResource(const Path& path, ScopedMountNode* out_node);

  
  
  virtual Error Unlink(const Path& path) = 0;
  virtual Error Mkdir(const Path& path, int permissions) = 0;
  virtual Error Rmdir(const Path& path) = 0;
  virtual Error Remove(const Path& path) = 0;
  virtual Error Rename(const Path& path, const Path& newpath) = 0;

  
  void OnNodeCreated(MountNode* node);

  
  void OnNodeDestroyed(MountNode* node);

 protected:
  
  int dev_;
  PepperInterface* ppapi_;  
  INodePool inode_pool_;

 private:
  
  
  friend class KernelObject;
  friend class KernelProxy;
  DISALLOW_COPY_AND_ASSIGN(Mount);
};

}  

#endif  
