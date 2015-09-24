// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_DEVICE_ORIENTATION_EVENT_PUMP_H_
#define CONTENT_RENDERER_DEVICE_ORIENTATION_EVENT_PUMP_H_

#include "base/memory/scoped_ptr.h"
#include "content/renderer/device_orientation/device_sensor_event_pump.h"
#include "content/renderer/shared_memory_seqlock_reader.h"
#include "third_party/WebKit/public/platform/WebDeviceOrientationData.h"

namespace blink {
class WebDeviceOrientationListener;
}

namespace content {

typedef SharedMemorySeqLockReader<blink::WebDeviceOrientationData>
    DeviceOrientationSharedMemoryReader;

class CONTENT_EXPORT DeviceOrientationEventPump : public DeviceSensorEventPump {
 public:
  
  
  static const double kOrientationThreshold;

  DeviceOrientationEventPump();
  explicit DeviceOrientationEventPump(int pump_delay_millis);
  virtual ~DeviceOrientationEventPump();

  
  
  bool SetListener(blink::WebDeviceOrientationListener* listener);

  
  virtual bool OnControlMessageReceived(const IPC::Message& message) OVERRIDE;

 protected:
  virtual void FireEvent() OVERRIDE;
  virtual bool InitializeReader(base::SharedMemoryHandle handle) OVERRIDE;
  virtual bool SendStartMessage() OVERRIDE;
  virtual bool SendStopMessage() OVERRIDE;

  bool ShouldFireEvent(const blink::WebDeviceOrientationData& data) const;

  blink::WebDeviceOrientationListener* listener_;
  blink::WebDeviceOrientationData data_;
  scoped_ptr<DeviceOrientationSharedMemoryReader> reader_;
};

}  

#endif  
