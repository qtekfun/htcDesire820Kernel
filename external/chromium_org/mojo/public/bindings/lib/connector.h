// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_BINDINGS_LIB_CONNECTOR_H_
#define MOJO_PUBLIC_BINDINGS_LIB_CONNECTOR_H_

#include "mojo/public/bindings/lib/bindings_support.h"
#include "mojo/public/bindings/lib/message.h"
#include "mojo/public/bindings/lib/message_queue.h"
#include "mojo/public/system/core_cpp.h"

namespace mojo {
namespace internal {

class Connector : public MessageReceiver {
 public:
  
  explicit Connector(ScopedMessagePipeHandle message_pipe);
  virtual ~Connector();

  
  
  
  void SetIncomingReceiver(MessageReceiver* receiver);

  
  
  bool encountered_error() const { return error_; }

  
  virtual bool Accept(Message* message) MOJO_OVERRIDE;

 private:
  class Callback : public BindingsSupport::AsyncWaitCallback {
   public:
    Callback();
    virtual ~Callback();

    void SetOwnerToNotify(Connector* owner);
    void SetAsyncWaitID(BindingsSupport::AsyncWaitID async_wait_id);

    virtual void OnHandleReady(MojoResult result) MOJO_OVERRIDE;

   private:
    Connector* owner_;
    BindingsSupport::AsyncWaitID async_wait_id_;
  };
  friend class Callback;

  void OnHandleReady(Callback* callback, MojoResult result);
  void WaitToReadMore();
  void WaitToWriteMore();
  void ReadMore();
  void WriteMore();
  void WriteOne(Message* message, bool* wait_to_write);

  ScopedMessagePipeHandle message_pipe_;
  MessageReceiver* incoming_receiver_;
  MessageQueue write_queue_;

  Callback read_callback_;
  Callback write_callback_;

  bool error_;

  MOJO_DISALLOW_COPY_AND_ASSIGN(Connector);
};

}  
}  

#endif  
