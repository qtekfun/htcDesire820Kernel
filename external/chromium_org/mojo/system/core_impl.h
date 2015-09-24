// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_CORE_IMPL_H_
#define MOJO_SYSTEM_CORE_IMPL_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "mojo/public/system/core_private.h"
#include "mojo/system/system_impl_export.h"

namespace mojo {
namespace system {

class CoreImpl;
class Dispatcher;

namespace test {
class CoreTestBase;
}

class MOJO_SYSTEM_IMPL_EXPORT CoreImpl : public CorePrivate {
 public:
  static void Init();

  
  virtual MojoTimeTicks GetTimeTicksNow() OVERRIDE;
  virtual MojoResult Close(MojoHandle handle) OVERRIDE;
  virtual MojoResult Wait(MojoHandle handle,
                          MojoWaitFlags flags,
                          MojoDeadline deadline) OVERRIDE;
  virtual MojoResult WaitMany(const MojoHandle* handles,
                              const MojoWaitFlags* flags,
                              uint32_t num_handles,
                              MojoDeadline deadline) OVERRIDE;
  virtual MojoResult CreateMessagePipe(
      MojoHandle* message_pipe_handle_0,
      MojoHandle* message_pipe_handle_1) OVERRIDE;
  virtual MojoResult WriteMessage(MojoHandle message_pipe_handle,
                                  const void* bytes,
                                  uint32_t num_bytes,
                                  const MojoHandle* handles,
                                  uint32_t num_handles,
                                  MojoWriteMessageFlags flags) OVERRIDE;
  virtual MojoResult ReadMessage(MojoHandle message_pipe_handle,
                                 void* bytes,
                                 uint32_t* num_bytes,
                                 MojoHandle* handles,
                                 uint32_t* num_handles,
                                 MojoReadMessageFlags flags) OVERRIDE;
  virtual MojoResult CreateDataPipe(
      const MojoCreateDataPipeOptions* options,
      MojoHandle* data_pipe_producer_handle,
      MojoHandle* data_pipe_consumer_handle) OVERRIDE;
  virtual MojoResult WriteData(MojoHandle data_pipe_producer_handle,
                               const void* elements,
                               uint32_t* num_elements,
                               MojoWriteDataFlags flags) OVERRIDE;
  virtual MojoResult BeginWriteData(MojoHandle data_pipe_producer_handle,
                                    void** buffer,
                                    uint32_t* buffer_num_elements,
                                    MojoWriteDataFlags flags) OVERRIDE;
  virtual MojoResult EndWriteData(MojoHandle data_pipe_producer_handle,
                                  uint32_t num_elements_written) OVERRIDE;
  virtual MojoResult ReadData(MojoHandle data_pipe_consumer_handle,
                              void* elements,
                              uint32_t* num_elements,
                              MojoReadDataFlags flags) OVERRIDE;
  virtual MojoResult BeginReadData(MojoHandle data_pipe_consumer_handle,
                                   const void** buffer,
                                   uint32_t* buffer_num_elements,
                                   MojoReadDataFlags flags) OVERRIDE;
  virtual MojoResult EndReadData(MojoHandle data_pipe_consumer_handle,
                                 uint32_t num_elements_read) OVERRIDE;

 private:
  friend class test::CoreTestBase;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  struct HandleTableEntry {
    HandleTableEntry();
    explicit HandleTableEntry(const scoped_refptr<Dispatcher>& dispatcher);
    ~HandleTableEntry();

    scoped_refptr<Dispatcher> dispatcher;
    bool busy;
  };
  typedef base::hash_map<MojoHandle, HandleTableEntry> HandleTableMap;

  CoreImpl();
  virtual ~CoreImpl();

  
  
  scoped_refptr<Dispatcher> GetDispatcher(MojoHandle handle);

  
  
  
  MojoHandle AddDispatcherNoLock(const scoped_refptr<Dispatcher>& dispatcher);

  
  
  MojoResult WaitManyInternal(const MojoHandle* handles,
                              const MojoWaitFlags* flags,
                              uint32_t num_handles,
                              MojoDeadline deadline);

  

  
  
  base::Lock handle_table_lock_;  
  HandleTableMap handle_table_;
  MojoHandle next_handle_;  

  

  DISALLOW_COPY_AND_ASSIGN(CoreImpl);
};

}  
}  

#endif  
