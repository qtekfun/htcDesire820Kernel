// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_CHANNEL_DISPATCHER_BASE_H_
#define REMOTING_PROTOCOL_CHANNEL_DISPATCHER_BASE_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"

namespace net {
class StreamSocket;
}  

namespace remoting {
namespace protocol {

struct ChannelConfig;
class ChannelFactory;
class Session;

class ChannelDispatcherBase {
 public:
  
  
  typedef base::Callback<void(bool)> InitializedCallback;

  virtual ~ChannelDispatcherBase();

  
  
  void Init(Session* session,
            const ChannelConfig& config,
            const InitializedCallback& callback);

  
  bool is_connected() { return channel() != NULL; }

 protected:
  explicit ChannelDispatcherBase(const char* channel_name);

  net::StreamSocket* channel() { return channel_.get(); }

  
  
  virtual void OnInitialized() = 0;

 private:
  void OnChannelReady(scoped_ptr<net::StreamSocket> socket);

  std::string channel_name_;
  ChannelFactory* channel_factory_;
  InitializedCallback initialized_callback_;
  scoped_ptr<net::StreamSocket> channel_;

  DISALLOW_COPY_AND_ASSIGN(ChannelDispatcherBase);
};

}  
}  

#endif  
