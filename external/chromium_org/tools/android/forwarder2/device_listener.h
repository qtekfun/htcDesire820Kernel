// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_ANDROID_FORWARDER2_DEVICE_LISTENER_H_
#define TOOLS_ANDROID_FORWARDER2_DEVICE_LISTENER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/thread.h"
#include "tools/android/forwarder2/pipe_notifier.h"
#include "tools/android/forwarder2/self_deleter_helper.h"
#include "tools/android/forwarder2/socket.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace forwarder2 {

class Forwarder;

class DeviceListener {
 public:
  
  
  
  typedef base::Callback<void (scoped_ptr<DeviceListener>)> ErrorCallback;

  static scoped_ptr<DeviceListener> Create(scoped_ptr<Socket> host_socket,
                                           int port,
                                           const ErrorCallback& error_callback);

  ~DeviceListener();

  void Start();

  void SetAdbDataSocket(scoped_ptr<Socket> adb_data_socket);

  int listener_port() const { return listener_port_; }

 private:
  DeviceListener(scoped_ptr<PipeNotifier> pipe_notifier,
                 scoped_ptr<Socket> listener_socket,
                 scoped_ptr<Socket> host_socket,
                 int port,
                 const ErrorCallback& error_callback);

  
  
  void AcceptNextClientSoon();

  void AcceptClientOnInternalThread();

  void OnAdbDataSocketReceivedOnInternalThread(
      scoped_ptr<Socket> adb_data_socket);

  void OnInternalThreadError();

  SelfDeleterHelper<DeviceListener> self_deleter_helper_;
  
  
  
  
  
  
  
  const scoped_ptr<PipeNotifier> exit_notifier_;
  
  
  const scoped_ptr<Socket> listener_socket_;
  
  const scoped_ptr<Socket> host_socket_;
  scoped_ptr<Socket> device_data_socket_;
  
  
  scoped_ptr<Socket> adb_data_socket_;
  const int listener_port_;
  
  
  scoped_refptr<base::SingleThreadTaskRunner> deletion_task_runner_;
  base::Thread thread_;

  DISALLOW_COPY_AND_ASSIGN(DeviceListener);
};

}  

#endif  
