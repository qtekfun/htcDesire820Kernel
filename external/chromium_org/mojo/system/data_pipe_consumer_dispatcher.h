// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_DATA_PIPE_CONSUMER_DISPATCHER_H_
#define MOJO_SYSTEM_DATA_PIPE_CONSUMER_DISPATCHER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "mojo/system/dispatcher.h"
#include "mojo/system/system_impl_export.h"

namespace mojo {
namespace system {

class DataPipe;

class MOJO_SYSTEM_IMPL_EXPORT DataPipeConsumerDispatcher : public Dispatcher {
 public:
  DataPipeConsumerDispatcher();

  
  void Init(scoped_refptr<DataPipe> data_pipe);

 private:
  friend class base::RefCountedThreadSafe<DataPipeConsumerDispatcher>;
  virtual ~DataPipeConsumerDispatcher();

  
  virtual void CancelAllWaitersNoLock() OVERRIDE;
  virtual MojoResult CloseImplNoLock() OVERRIDE;
  virtual MojoResult ReadDataImplNoLock(void* elements,
                                        uint32_t* num_elements,
                                        MojoReadDataFlags flags) OVERRIDE;
  virtual MojoResult BeginReadDataImplNoLock(const void** buffer,
                                             uint32_t* buffer_num_elements,
                                             MojoReadDataFlags flags) OVERRIDE;
  virtual MojoResult EndReadDataImplNoLock(uint32_t num_elements_read) OVERRIDE;
  virtual MojoResult AddWaiterImplNoLock(Waiter* waiter,
                                         MojoWaitFlags flags,
                                         MojoResult wake_result) OVERRIDE;
  virtual void RemoveWaiterImplNoLock(Waiter* waiter) OVERRIDE;
  virtual scoped_refptr<Dispatcher>
      CreateEquivalentDispatcherAndCloseImplNoLock() OVERRIDE;

  
  scoped_refptr<DataPipe> data_pipe_;  

  DISALLOW_COPY_AND_ASSIGN(DataPipeConsumerDispatcher);
};

}  
}  

#endif  
