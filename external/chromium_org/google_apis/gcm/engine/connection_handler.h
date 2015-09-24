// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GCM_ENGINE_CONNECTION_HANDLER_H_
#define GOOGLE_APIS_GCM_ENGINE_CONNECTION_HANDLER_H_

#include "base/callback.h"
#include "google_apis/gcm/base/gcm_export.h"

namespace net{
class StreamSocket;
}  

namespace google {
namespace protobuf {
class MessageLite;
}  
}  

namespace mcs_proto {
class LoginRequest;
}

namespace gcm {

class SocketInputStream;
class SocketOutputStream;

class GCM_EXPORT ConnectionHandler {
 public:
  typedef base::Callback<void(scoped_ptr<google::protobuf::MessageLite>)>
      ProtoReceivedCallback;
  typedef base::Closure ProtoSentCallback;
  typedef base::Callback<void(int)> ConnectionChangedCallback;

  ConnectionHandler();
  virtual ~ConnectionHandler();

  
  
  
  
  
  virtual void Init(const mcs_proto::LoginRequest& login_request,
                    scoped_ptr<net::StreamSocket> socket) = 0;

  
  
  virtual bool CanSendMessage() const = 0;

  
  virtual void SendMessage(const google::protobuf::MessageLite& message) = 0;
};

}  

#endif  
