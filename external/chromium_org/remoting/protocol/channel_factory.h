// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_CHANNEL_FACTORY_H_
#define REMOTING_PROTOCOL_CHANNEL_FACTORY_H_

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"

namespace net {
class Socket;
class StreamSocket;
}  

namespace remoting {
namespace protocol {

class ChannelFactory : public base::NonThreadSafe {
 public:
  
  
  typedef base::Callback<void(scoped_ptr<net::StreamSocket>)>
      StreamChannelCallback;
  typedef base::Callback<void(scoped_ptr<net::Socket>)>
      DatagramChannelCallback;

  ChannelFactory() {}

  
  
  
  
  
  
  
  virtual void CreateStreamChannel(
      const std::string& name, const StreamChannelCallback& callback) = 0;
  virtual void CreateDatagramChannel(
      const std::string& name, const DatagramChannelCallback& callback) = 0;

  
  
  
  
  virtual void CancelChannelCreation(const std::string& name) = 0;

 protected:
  virtual ~ChannelFactory() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(ChannelFactory);
};

}  
}  

#endif  
