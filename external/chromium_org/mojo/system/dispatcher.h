// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_DISPATCHER_H_
#define MOJO_SYSTEM_DISPATCHER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "mojo/public/system/core.h"
#include "mojo/system/system_impl_export.h"

namespace mojo {
namespace system {

class CoreImpl;
class Waiter;

class MOJO_SYSTEM_IMPL_EXPORT Dispatcher :
    public base::RefCountedThreadSafe<Dispatcher> {
 public:
  
  
  
  
  
  
  
  MojoResult Close();
  
  // written, in which case this will be called with all the dispatchers' locks
  
  
  MojoResult WriteMessage(const void* bytes,
                          uint32_t num_bytes,
                          const std::vector<Dispatcher*>* dispatchers,
                          MojoWriteMessageFlags flags);
  
  
  
  MojoResult ReadMessage(
      void* bytes,
      uint32_t* num_bytes,
      std::vector<scoped_refptr<Dispatcher> >* dispatchers,
      uint32_t* num_dispatchers,
      MojoReadMessageFlags flags);
  MojoResult WriteData(const void* elements,
                       uint32_t* num_elements,
                       MojoWriteDataFlags flags);
  MojoResult BeginWriteData(void** buffer,
                            uint32_t* buffer_num_elements,
                            MojoWriteDataFlags flags);
  MojoResult EndWriteData(uint32_t num_elements_written);
  MojoResult ReadData(void* elements,
                      uint32_t* num_elements,
                      MojoReadDataFlags flags);
  MojoResult BeginReadData(const void** buffer,
                           uint32_t* buffer_num_elements,
                           MojoReadDataFlags flags);
  MojoResult EndReadData(uint32_t num_elements_read);

  
  
  
  
  
  
  
  
  
  
  
  
  MojoResult AddWaiter(Waiter* waiter,
                       MojoWaitFlags flags,
                       MojoResult wake_result);
  void RemoveWaiter(Waiter* waiter);

  
  
  
  
  
  scoped_refptr<Dispatcher> CreateEquivalentDispatcherAndCloseNoLock();

 protected:
  Dispatcher();

  friend class base::RefCountedThreadSafe<Dispatcher>;
  virtual ~Dispatcher();

  
  
  
  virtual void CancelAllWaitersNoLock();
  virtual MojoResult CloseImplNoLock();

  
  
  
  
  virtual MojoResult WriteMessageImplNoLock(
      const void* bytes,
      uint32_t num_bytes,
      const std::vector<Dispatcher*>* dispatchers,
      MojoWriteMessageFlags flags);
  virtual MojoResult ReadMessageImplNoLock(
      void* bytes,
      uint32_t* num_bytes,
      std::vector<scoped_refptr<Dispatcher> >* dispatchers,
      uint32_t* num_dispatchers,
      MojoReadMessageFlags flags);
  virtual MojoResult WriteDataImplNoLock(const void* elements,
                                         uint32_t* num_elements,
                                         MojoWriteDataFlags flags);
  virtual MojoResult BeginWriteDataImplNoLock(void** buffer,
                                              uint32_t* buffer_num_elements,
                                              MojoWriteDataFlags flags);
  virtual MojoResult EndWriteDataImplNoLock(uint32_t num_elements_written);
  virtual MojoResult ReadDataImplNoLock(void* elements,
                                        uint32_t* num_elements,
                                        MojoReadDataFlags flags);
  virtual MojoResult BeginReadDataImplNoLock(const void** buffer,
                                             uint32_t* buffer_num_elements,
                                             MojoReadDataFlags flags);
  virtual MojoResult EndReadDataImplNoLock(uint32_t num_elements_read);
  virtual MojoResult AddWaiterImplNoLock(Waiter* waiter,
                                         MojoWaitFlags flags,
                                         MojoResult wake_result);
  virtual void RemoveWaiterImplNoLock(Waiter* waiter);

  
  
  
  virtual scoped_refptr<Dispatcher>
      CreateEquivalentDispatcherAndCloseImplNoLock() = 0;

  
  
  base::Lock& lock() const { return lock_; }

  bool is_closed_no_lock() const { return is_closed_; }

 private:
  
  
  friend class CoreImpl;

  
  
  mutable base::Lock lock_;
  bool is_closed_;

  DISALLOW_COPY_AND_ASSIGN(Dispatcher);
};

}  
}  

#endif  
