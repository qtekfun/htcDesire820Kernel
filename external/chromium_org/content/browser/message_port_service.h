// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MESSAGE_PORT_SERVICE_H_
#define CONTENT_BROWSER_MESSAGE_PORT_SERVICE_H_

#include <map>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/singleton.h"
#include "base/strings/string16.h"
#include "ipc/ipc_message.h"

namespace content {
class MessagePortMessageFilter;

class MessagePortService {
 public:
  typedef std::vector<std::pair<base::string16, std::vector<int> > >
      QueuedMessages;

  
  static MessagePortService* GetInstance();

  
  void Create(int route_id,
              MessagePortMessageFilter* filter,
              int* message_port_id);
  void Destroy(int message_port_id);
  void Entangle(int local_message_port_id, int remote_message_port_id);
  void PostMessage(int sender_message_port_id,
                   const base::string16& message,
                   const std::vector<int>& sent_message_port_ids);
  void QueueMessages(int message_port_id);
  void SendQueuedMessages(int message_port_id,
                          const QueuedMessages& queued_messages);

  
  
  void UpdateMessagePort(
      int message_port_id,
      MessagePortMessageFilter* filter,
      int routing_id);

  void OnMessagePortMessageFilterClosing(MessagePortMessageFilter* filter);

  
  void SendQueuedMessagesIfPossible(int message_port_id);

 private:
  friend struct DefaultSingletonTraits<MessagePortService>;

  MessagePortService();
  ~MessagePortService();

  void PostMessageTo(int message_port_id,
                     const base::string16& message,
                     const std::vector<int>& sent_message_port_ids);

  
  
  void Erase(int message_port_id);

  struct MessagePort;
  typedef std::map<int, MessagePort> MessagePorts;
  MessagePorts message_ports_;

  
  int next_message_port_id_;

  DISALLOW_COPY_AND_ASSIGN(MessagePortService);
};

}  

#endif  
