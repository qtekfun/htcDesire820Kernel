// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_RESOURCE_MESSAGE_TEST_SINK_H_
#define PPAPI_PROXY_RESOURCE_MESSAGE_TEST_SINK_H_

#include "ipc/ipc_listener.h"
#include "ipc/ipc_test_sink.h"
#include "ppapi/c/pp_stdint.h"

namespace ppapi {
namespace proxy {

class ResourceMessageCallParams;
class ResourceMessageReplyParams;
class SerializedHandle;

class ResourceMessageTestSink : public IPC::TestSink {
 public:
  ResourceMessageTestSink();
  virtual ~ResourceMessageTestSink();

  
  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  
  void SetSyncReplyMessage(IPC::Message* reply_msg);

  
  
  
  bool GetFirstResourceCallMatching(
      uint32 id,
      ResourceMessageCallParams* params,
      IPC::Message* nested_msg) const;

  
  bool GetFirstResourceReplyMatching(
      uint32 id,
      ResourceMessageReplyParams* params,
      IPC::Message* nested_msg);

 private:
  scoped_ptr<IPC::Message> sync_reply_msg_;
};

class ResourceSyncCallHandler : public IPC::Listener {
 public:
  ResourceSyncCallHandler(ResourceMessageTestSink* test_sink,
                          uint32 incoming_type,
                          int32_t result,
                          const IPC::Message& reply_msg);
  virtual ~ResourceSyncCallHandler();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  IPC::Message last_handled_msg() { return last_handled_msg_; }

  
  
  void set_serialized_handle(const SerializedHandle* serialized_handle) {
    serialized_handle_ = serialized_handle;
  }

 private:
  ResourceMessageTestSink* test_sink_;
  uint32 incoming_type_;
  int32_t result_;
  const SerializedHandle* serialized_handle_;  
  IPC::Message reply_msg_;
  IPC::Message last_handled_msg_;
};

}  
}  

#endif  
