// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_ANDROID_FORWARDER2_DEVICE_CONTROLLER_H_
#define TOOLS_ANDROID_FORWARDER2_DEVICE_CONTROLLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "tools/android/forwarder2/socket.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace forwarder2 {

class DeviceListener;

class DeviceController {
 public:
  static scoped_ptr<DeviceController> Create(const std::string& adb_unix_socket,
                                             int exit_notifier_fd);
  ~DeviceController();

  void Start();

 private:
  typedef base::hash_map<
      int , linked_ptr<DeviceListener> > ListenersMap;

  DeviceController(scoped_ptr<Socket> host_socket, int exit_notifier_fd);

  void AcceptHostCommandSoon();
  void AcceptHostCommandInternal();

  
  
  static void DeleteListenerOnError(
      const base::WeakPtr<DeviceController>& device_controller_ptr,
      scoped_ptr<DeviceListener> device_listener);

  const scoped_ptr<Socket> host_socket_;
  
  const int exit_notifier_fd_;
  
  
  const scoped_refptr<base::SingleThreadTaskRunner> construction_task_runner_;
  base::WeakPtrFactory<DeviceController> weak_ptr_factory_;
  ListenersMap listeners_;

  DISALLOW_COPY_AND_ASSIGN(DeviceController);
};

}  

#endif  
