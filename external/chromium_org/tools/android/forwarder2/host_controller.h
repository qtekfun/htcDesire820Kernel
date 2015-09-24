// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_ANDROID_FORWARDER2_HOST_CONTROLLER_H_
#define TOOLS_ANDROID_FORWARDER2_HOST_CONTROLLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/thread.h"
#include "tools/android/forwarder2/pipe_notifier.h"
#include "tools/android/forwarder2/self_deleter_helper.h"
#include "tools/android/forwarder2/socket.h"

namespace forwarder2 {

class HostController {
 public:
  
  
  typedef base::Callback<void (scoped_ptr<HostController>)> ErrorCallback;

  
  
  static scoped_ptr<HostController> Create(int device_port,
                                           int host_port,
                                           int adb_port,
                                           int exit_notifier_fd,
                                           const ErrorCallback& error_callback);

  ~HostController();

  
  void Start();

  int adb_port() const { return adb_port_; }

  int device_port() const { return device_port_; }

 private:
  HostController(int device_port,
                 int host_port,
                 int adb_port,
                 int exit_notifier_fd,
                 const ErrorCallback& error_callback,
                 scoped_ptr<Socket> adb_control_socket,
                 scoped_ptr<PipeNotifier> delete_controller_notifier);

  void ReadNextCommandSoon();
  void ReadCommandOnInternalThread();

  void StartForwarder(scoped_ptr<Socket> host_server_data_socket);

  
  
  scoped_ptr<Socket> CreateSocket();

  
  void OnInternalThreadError();

  void UnmapPortOnDevice();

  SelfDeleterHelper<HostController> self_deleter_helper_;
  const int device_port_;
  const int host_port_;
  const int adb_port_;
  
  const int global_exit_notifier_fd_;
  scoped_ptr<Socket> adb_control_socket_;
  scoped_ptr<PipeNotifier> delete_controller_notifier_;
  
  
  
  
  const scoped_refptr<base::SingleThreadTaskRunner> deletion_task_runner_;
  base::Thread thread_;

  DISALLOW_COPY_AND_ASSIGN(HostController);
};

}  

#endif  
