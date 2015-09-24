// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TEST_SPAWNED_TEST_SERVER_SPAWNER_COMMUNICATOR_H_
#define NET_TEST_SPAWNED_TEST_SERVER_SPAWNER_COMMUNICATOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "net/url_request/url_request.h"

namespace net {

class ScopedPortException;

// The test server used by unit tests is written in Python. However, Android
class SpawnerCommunicator : public net::URLRequest::Delegate {
 public:
  explicit SpawnerCommunicator(uint16 port);
  virtual ~SpawnerCommunicator();

  
  
  
  
  bool StartServer(const std::string& arguments,
                   uint16* port) WARN_UNUSED_RESULT;

  bool StopServer() WARN_UNUSED_RESULT;

 private:
  
  void StartIOThread();

  
  void Shutdown();

  
  void WaitForResponse();

  
  
  
  
  
  
  void SendCommandAndWaitForResult(const std::string& command,
                                   const std::string& post_data,
                                   int* result_code,
                                   std::string* data_received);

  
  void SendCommandAndWaitForResultOnIOThread(const std::string& command,
                                             const std::string& post_data,
                                             int* result_code,
                                             std::string* data_received);

  
  virtual void OnResponseStarted(URLRequest* request) OVERRIDE;
  virtual void OnReadCompleted(URLRequest* request, int num_bytes) OVERRIDE;

  
  void ReadResult(URLRequest* request);

  
  void OnSpawnerCommandCompleted(URLRequest* request);

  
  void OnTimeout(int id);

  
  base::Thread io_thread_;

  
  base::WaitableEvent event_;

  
  
  
  
  const uint16 port_;

  
  scoped_ptr<ScopedPortException> allowed_port_;

  
  int next_id_;

  
  
  base::WeakPtrFactory<SpawnerCommunicator> weak_factory_;

  
  scoped_ptr<URLRequestContext> context_;

  
  scoped_ptr<URLRequest> cur_request_;

  
  bool is_running_;

  DISALLOW_COPY_AND_ASSIGN(SpawnerCommunicator);
};

}  

#endif  
