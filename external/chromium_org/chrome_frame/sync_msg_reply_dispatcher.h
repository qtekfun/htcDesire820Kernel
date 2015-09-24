// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_SYNC_MSG_REPLY_DISPATCHER_H_
#define CHROME_FRAME_SYNC_MSG_REPLY_DISPATCHER_H_

#include <deque>

#include "base/synchronization/lock.h"
#include "ipc/ipc_channel_proxy.h"

class SyncMessageReplyDispatcher : public IPC::ChannelProxy::MessageFilter {
 public:
  class SyncMessageCallContext {
   public:
    SyncMessageCallContext()
        : id_(0),
          message_type_(0),
          key_(NULL) {}

    virtual ~SyncMessageCallContext() {}

    uint32 message_type() const {
      return message_type_;
    }

   private:
    int id_;
    uint32 message_type_;
    void* key_;

    friend class SyncMessageReplyDispatcher;
  };

  SyncMessageReplyDispatcher() {}
  void Push(IPC::SyncMessage* msg, SyncMessageCallContext* context,
            void* key);
  void Cancel(void* key);

 protected:
  typedef std::deque<SyncMessageCallContext*> PendingSyncMessageQueue;

  SyncMessageCallContext* GetContext(const IPC::Message& msg);

  virtual bool OnMessageReceived(const IPC::Message& msg);

  
  
  
  
  virtual bool HandleMessageType(const IPC::Message& msg,
                                 SyncMessageCallContext* context);

  template <typename T>
  void InvokeCallback(const IPC::Message& msg,
                      SyncMessageCallContext* call_context) {
    if (!call_context || !call_context->key_) {
      NOTREACHED() << "Invalid context parameter";
      return;
    }

    T* context = static_cast<T*>(call_context);
    T::output_type tmp;  
    IPC::ParamDeserializer<T::output_type> deserializer(tmp);
    if (deserializer.MessageReplyDeserializer::SerializeOutputParameters(msg)) {
      DispatchToMethod(context, &T::Completed, deserializer.out_);
      delete context;
    } else {
      
    }
  }

  PendingSyncMessageQueue message_queue_;
  base::Lock message_queue_lock_;
};

#endif  
