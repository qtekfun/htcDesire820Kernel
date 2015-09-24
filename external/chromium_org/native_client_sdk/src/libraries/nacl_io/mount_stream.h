// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRARIES_NACL_IO_MOUNT_STREAM_H_
#define LIBRARIES_NACL_IO_MOUNT_STREAM_H_

#include "nacl_io/mount.h"

#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_resource.h"


namespace nacl_io {


class MountStream;
class MountNodeStream;

class MountStream : public Mount {
 public:
  class Work {
   public:
    explicit Work(MountStream* mount) : mount_(mount) {}
    virtual ~Work() {}

    
    virtual bool Start(int32_t val) = 0;

    
    
    virtual void Run(int32_t val) = 0;
    MountStream* mount() { return mount_; }

  private:
    MountStream* mount_;
  };

 protected:
  MountStream();
  virtual ~MountStream();

 public:
  
  void EnqueueWork(Work* work);

  
  
  static PP_CompletionCallback GetStartCompletion(Work* work);

  
  
  static PP_CompletionCallback GetRunCompletion(Work* work);

  virtual Error Access(const Path& path, int a_mode);
  virtual Error Open(const Path& path,
                     int o_flags,
                     ScopedMountNode* out_node);
  virtual Error Unlink(const Path& path);
  virtual Error Mkdir(const Path& path, int permissions);
  virtual Error Rmdir(const Path& path);
  virtual Error Remove(const Path& path);
  virtual Error Rename(const Path& path, const Path& newpath);

  static void* StreamThreadThunk(void*);
  void StreamThread();

 private:
  PP_Resource message_loop_;
  pthread_cond_t message_cond_;
  sdk_util::SimpleLock message_lock_;

  friend class KernelProxy;
  DISALLOW_COPY_AND_ASSIGN(MountStream);
};

}  

#endif  
