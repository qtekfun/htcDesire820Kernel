// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_TEST_SINK_H_
#define IPC_IPC_TEST_SINK_H_

#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "ipc/ipc_channel.h"

namespace IPC {

class Message;

class TestSink : public Channel {
 public:
  TestSink();
  virtual ~TestSink();

  
  
  virtual bool Send(IPC::Message* message) OVERRIDE;

  
  
  bool OnMessageReceived(const Message& msg);

  
  size_t message_count() const { return messages_.size(); }

  
  void ClearMessages();

  
  
  
  const Message* GetMessageAt(size_t index) const;

  
  
  
  const Message* GetFirstMessageMatching(uint32 id) const;

  
  
  
  
  
  const Message* GetUniqueMessageMatching(uint32 id) const;

  
  
  
  
  
  void AddFilter(Listener* filter);

  
  void RemoveFilter(Listener* filter);

 private:
  
  std::vector<Message> messages_;
  ObserverList<Listener> filter_list_;

  DISALLOW_COPY_AND_ASSIGN(TestSink);
};

}  

#endif  
