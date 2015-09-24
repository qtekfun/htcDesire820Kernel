// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_DATA_PIPE_H_
#define MOJO_SYSTEM_DATA_PIPE_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "mojo/public/system/core.h"
#include "mojo/system/system_impl_export.h"

namespace mojo {
namespace system {

class Waiter;
class WaiterList;

class MOJO_SYSTEM_IMPL_EXPORT DataPipe :
    public base::RefCountedThreadSafe<DataPipe> {
 public:
  
  
  void ProducerCancelAllWaiters();
  void ProducerClose();
  
  MojoResult ProducerWriteData(const void* elements,
                               uint32_t* num_elements,
                               MojoWriteDataFlags flags);
  
  MojoResult ProducerBeginWriteData(void** buffer,
                                    uint32_t* buffer_num_elements,
                                    MojoWriteDataFlags flags);
  MojoResult ProducerEndWriteData(uint32_t num_elements_written);
  MojoResult ProducerAddWaiter(Waiter* waiter,
                               MojoWaitFlags flags,
                               MojoResult wake_result);
  void ProducerRemoveWaiter(Waiter* waiter);

  
  
  void ConsumerCancelAllWaiters();
  void ConsumerClose();
  
  MojoResult ConsumerReadData(void* elements,
                              uint32_t* num_elements,
                              MojoReadDataFlags flags);
  
  MojoResult ConsumerBeginReadData(const void** buffer,
                                   uint32_t* buffer_num_elements,
                                   MojoReadDataFlags flags);
  MojoResult ConsumerEndReadData(uint32_t num_elements_read);
  MojoResult ConsumerAddWaiter(Waiter* waiter,
                               MojoWaitFlags flags,
                               MojoResult wake_result);
  void ConsumerRemoveWaiter(Waiter* waiter);

  
  bool may_discard() const { return may_discard_; }
  size_t element_size() const { return element_size_; }
  size_t capacity_num_elements() const { return capacity_num_elements_; }

 protected:
  DataPipe(bool has_local_producer, bool has_local_consumer);

  friend class base::RefCountedThreadSafe<DataPipe>;
  virtual ~DataPipe();

  
  
  MojoResult Init(bool may_discard,
                  size_t element_size,
                  size_t capacity_num_elements);

  void AwakeProducerWaitersForStateChangeNoLock();
  void AwakeConsumerWaitersForStateChangeNoLock();

  virtual void ProducerCloseImplNoLock() = 0;
  virtual MojoResult ProducerBeginWriteDataImplNoLock(
      void** buffer,
      uint32_t* buffer_num_elements,
      MojoWriteDataFlags flags) = 0;
  virtual MojoResult ProducerEndWriteDataImplNoLock(
      uint32_t num_elements_written) = 0;
  virtual MojoWaitFlags ProducerSatisfiedFlagsNoLock() = 0;
  virtual MojoWaitFlags ProducerSatisfiableFlagsNoLock() = 0;

  virtual void ConsumerCloseImplNoLock() = 0;
  virtual MojoResult ConsumerDiscardDataNoLock(uint32_t* num_elements,
                                               bool all_or_none) = 0;
  virtual MojoResult ConsumerQueryDataNoLock(uint32_t* num_elements) = 0;
  virtual MojoResult ConsumerBeginReadDataImplNoLock(
      const void** buffer,
      uint32_t* buffer_num_elements,
      MojoReadDataFlags flags) = 0;
  virtual MojoResult ConsumerEndReadDataImplNoLock(
      uint32_t num_elements_read) = 0;
  virtual MojoWaitFlags ConsumerSatisfiedFlagsNoLock() = 0;
  virtual MojoWaitFlags ConsumerSatisfiableFlagsNoLock() = 0;

 private:
  bool has_local_producer_no_lock() const {
    return !!producer_waiter_list_.get();
  }
  bool has_local_consumer_no_lock() const {
    return !!consumer_waiter_list_.get();
  }

  
  bool may_discard_;
  size_t element_size_;
  size_t capacity_num_elements_;

  base::Lock lock_;  
  scoped_ptr<WaiterList> producer_waiter_list_;
  scoped_ptr<WaiterList> consumer_waiter_list_;
  bool producer_in_two_phase_write_;
  bool consumer_in_two_phase_read_;

  DISALLOW_COPY_AND_ASSIGN(DataPipe);
};

}  
}  

#endif  
