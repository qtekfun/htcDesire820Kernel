// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_CHANNEL_AUTHENTICATOR_H_
#define REMOTING_PROTOCOL_CHANNEL_AUTHENTICATOR_H_

#include <string>

#include "base/callback_forward.h"
#include "net/base/net_errors.h"

namespace net {
class StreamSocket;
}  

namespace remoting {
namespace protocol {

class ChannelAuthenticator {
 public:
  typedef base::Callback<void(net::Error error, scoped_ptr<net::StreamSocket>)>
      DoneCallback;

  virtual ~ChannelAuthenticator() {}

  
  
  
  virtual void SecureAndAuthenticate(
      scoped_ptr<net::StreamSocket> socket,
      const DoneCallback& done_callback) = 0;
};

}  
}  

#endif  
