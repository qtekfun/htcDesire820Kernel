// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DEVICE_ORIENTATION_DEVICE_INERTIAL_SENSOR_SERVICE_H_
#define CONTENT_BROWSER_DEVICE_ORIENTATION_DEVICE_INERTIAL_SENSOR_SERVICE_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/memory/singleton.h"
#include "base/threading/thread_checker.h"
#include "content/browser/device_orientation/inertial_sensor_consts.h"
#include "content/common/content_export.h"

namespace content {

class DataFetcherSharedMemory;
class RenderProcessHost;

class CONTENT_EXPORT DeviceInertialSensorService {
 public:
  
  static DeviceInertialSensorService* GetInstance();

  
  
  
  void AddConsumer(ConsumerType consumer_type);

  
  
  void RemoveConsumer(ConsumerType cosumer_type);

  
  
  base::SharedMemoryHandle GetSharedMemoryHandleForProcess(
      ConsumerType consumer_type, base::ProcessHandle handle);

  
  void Shutdown();

  
  
  void SetDataFetcherForTests(DataFetcherSharedMemory* test_data_fetcher);

 private:
  friend struct DefaultSingletonTraits<DeviceInertialSensorService>;

  DeviceInertialSensorService();
  virtual ~DeviceInertialSensorService();

  bool ChangeNumberConsumers(ConsumerType consumer_type,
      int delta);
  int GetNumberConsumers(ConsumerType consumer_type) const;

  int num_motion_readers_;
  int num_orientation_readers_;
  bool is_shutdown_;
  scoped_ptr<DataFetcherSharedMemory> data_fetcher_;
  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(DeviceInertialSensorService);
};

}  

#endif  
