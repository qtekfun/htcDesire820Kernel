// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_SYNC_MESSAGE_H_
#define IPC_IPC_SYNC_MESSAGE_H_

#if defined(OS_WIN)
#include <windows.h>
#endif
#include <string>
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ipc/ipc_message.h"

namespace base {
class WaitableEvent;
}

namespace IPC {

class MessageReplyDeserializer;

class IPC_EXPORT SyncMessage : public Message {
 public:
  SyncMessage(int32 routing_id, uint32 type, PriorityValue priority,
              MessageReplyDeserializer* deserializer);
  virtual ~SyncMessage();

  
  
  
  MessageReplyDeserializer* GetReplyDeserializer();

  
  
  
  
  
  
  void set_pump_messages_event(base::WaitableEvent* event) {
    pump_messages_event_ = event;
    if (event) {
      header()->flags |= PUMPING_MSGS_BIT;
    } else {
      header()->flags &= ~PUMPING_MSGS_BIT;
    }
  }

  
  
  void EnableMessagePumping();

  base::WaitableEvent* pump_messages_event() const {
    return pump_messages_event_;
  }

  
  static bool IsMessageReplyTo(const Message& msg, int request_id);

  
  
  static PickleIterator GetDataIterator(const Message* msg);

  
  static int GetMessageId(const Message& msg);

  
  static Message* GenerateReply(const Message* msg);

 private:
  struct SyncHeader {
    
    int message_id;
  };

  static bool ReadSyncHeader(const Message& msg, SyncHeader* header);
  static bool WriteSyncHeader(Message* msg, const SyncHeader& header);

  scoped_ptr<MessageReplyDeserializer> deserializer_;
  base::WaitableEvent* pump_messages_event_;
};

class IPC_EXPORT MessageReplyDeserializer {
 public:
  virtual ~MessageReplyDeserializer() {}
  bool SerializeOutputParameters(const Message& msg);
 private:
  
  
  virtual bool SerializeOutputParameters(const Message& msg,
                                         PickleIterator iter) = 0;
};

struct PendingSyncMsg {
  PendingSyncMsg(int id,
                 MessageReplyDeserializer* d,
                 base::WaitableEvent* e)
      : id(id), deserializer(d), done_event(e), send_result(false) { }
  int id;
  MessageReplyDeserializer* deserializer;
  base::WaitableEvent* done_event;
  bool send_result;
};

}  

#endif  
