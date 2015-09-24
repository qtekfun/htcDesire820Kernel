// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GCM_ENGINE_CONNECTION_HANDLER_IMPL_H_
#define GOOGLE_APIS_GCM_ENGINE_CONNECTION_HANDLER_IMPL_H_

#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "google_apis/gcm/engine/connection_handler.h"

namespace mcs_proto {
class LoginRequest;
}  

namespace gcm {

class GCM_EXPORT ConnectionHandlerImpl : public ConnectionHandler {
 public:
  
  
  
  
  
  
  
  ConnectionHandlerImpl(
      base::TimeDelta read_timeout,
      const ProtoReceivedCallback& read_callback,
      const ProtoSentCallback& write_callback,
      const ConnectionChangedCallback& connection_callback);
  virtual ~ConnectionHandlerImpl();

  
  virtual void Init(const mcs_proto::LoginRequest& login_request,
                    scoped_ptr<net::StreamSocket> socket) OVERRIDE;
  virtual bool CanSendMessage() const OVERRIDE;
  virtual void SendMessage(const google::protobuf::MessageLite& message)
      OVERRIDE;

 private:
  
  enum ProcessingState {
    
    
    MCS_VERSION_TAG_AND_SIZE = 0,
    
    
    MCS_TAG_AND_SIZE,
    
    
    
    MCS_FULL_SIZE,
    
    
    MCS_PROTO_BYTES
  };

  
  
  void Login(const google::protobuf::MessageLite& login_request);

  
  void OnMessageSent();

  
  
  void GetNextMessage();

  
  
  
  
  void WaitForData(ProcessingState state);

  
  void OnGotVersion();
  void OnGotMessageTag();
  void OnGotMessageSize();
  void OnGotMessageBytes();

  
  void OnTimeout();

  
  void CloseConnection();

  
  
  
  
  
  
  const base::TimeDelta read_timeout_;
  base::OneShotTimer<ConnectionHandlerImpl> read_timeout_timer_;

  
  scoped_ptr<net::StreamSocket> socket_;
  scoped_ptr<SocketInputStream> input_stream_;
  scoped_ptr<SocketOutputStream> output_stream_;

  
  
  bool handshake_complete_;

  
  uint8 message_tag_;
  uint32 message_size_;

  ProtoReceivedCallback read_callback_;
  ProtoSentCallback write_callback_;
  ConnectionChangedCallback connection_callback_;

  base::WeakPtrFactory<ConnectionHandlerImpl> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ConnectionHandlerImpl);
};

}  

#endif  
