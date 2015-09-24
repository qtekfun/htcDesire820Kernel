// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DEVICE_ORIENTATION_DATA_FETCHER_SHARED_MEMORY_BASE_H_
#define CONTENT_BROWSER_DEVICE_ORIENTATION_DATA_FETCHER_SHARED_MEMORY_BASE_H_

#include <map>

#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/message_loop/message_loop.h"
#include "content/browser/device_orientation/inertial_sensor_consts.h"
#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT DataFetcherSharedMemoryBase {
 public:

  
  
  
  bool StartFetchingDeviceData(ConsumerType consumer_type);

  
  
  bool StopFetchingDeviceData(ConsumerType consumer_type);

  
  
  
  
  base::SharedMemoryHandle GetSharedMemoryHandleForProcess(
      ConsumerType consumer_type, base::ProcessHandle process);

  enum FetcherType {
    
    FETCHER_TYPE_DEFAULT,
    
    FETCHER_TYPE_POLLING_CALLBACK,
    
    FETCHER_TYPE_SEPARATE_THREAD
  };

 protected:
  class PollingThread;

  DataFetcherSharedMemoryBase();
  virtual ~DataFetcherSharedMemoryBase();

  
  
  base::MessageLoop* GetPollingMessageLoop() const;

  
  
  virtual void Fetch(unsigned consumer_bitmask);

  
  virtual FetcherType GetType() const;

  
  
  
  virtual base::TimeDelta GetInterval() const;

  
  
  
  virtual bool Start(ConsumerType consumer_type, void* buffer) = 0;
  virtual bool Stop(ConsumerType consumer_type) = 0;

  bool IsPollingTimerRunningForTesting() const;

 private:
  bool InitAndStartPollingThreadIfNecessary();
  base::SharedMemory* GetSharedMemory(ConsumerType consumer_type);
  void* GetSharedMemoryBuffer(ConsumerType consumer_type);

  unsigned started_consumers_;

  scoped_ptr<PollingThread> polling_thread_;

  
  typedef std::map<ConsumerType, base::SharedMemory*> SharedMemoryMap;
  SharedMemoryMap shared_memory_map_;

  DISALLOW_COPY_AND_ASSIGN(DataFetcherSharedMemoryBase);
};

}

#endif  
