// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GCM_ENGINE_FAKE_CONNECTION_HANDLER_H_
#define GOOGLE_APIS_GCM_ENGINE_FAKE_CONNECTION_HANDLER_H_

#include <list>

#include "google_apis/gcm/base/mcs_message.h"
#include "google_apis/gcm/engine/connection_handler.h"

namespace gcm {

class FakeConnectionHandler : public ConnectionHandler {
 public:
  FakeConnectionHandler(
      const ConnectionHandler::ProtoReceivedCallback& read_callback,
      const ConnectionHandler::ProtoSentCallback& write_callback);
  virtual ~FakeConnectionHandler();

  
  virtual void Init(const mcs_proto::LoginRequest& login_request,
                    scoped_ptr<net::StreamSocket> socket) OVERRIDE;
  virtual bool CanSendMessage() const OVERRIDE;
  virtual void SendMessage(const google::protobuf::MessageLite& message)
      OVERRIDE;

  
  void ExpectOutgoingMessage(const MCSMessage& message);

  
  void ResetOutgoingMessageExpectations();

  
  bool AllOutgoingMessagesReceived() const;

  
  void ReceiveMessage(const MCSMessage& message);

  
  void set_fail_login(bool fail_login) {
    fail_login_ = fail_login;
  }

  
  
  void set_fail_send(bool fail_send) {
    fail_send_ = fail_send;
  }

 private:
  ConnectionHandler::ProtoReceivedCallback read_callback_;
  ConnectionHandler::ProtoSentCallback write_callback_;

  std::list<MCSMessage> expected_outgoing_messages_;

  
  bool fail_login_;

  
  bool fail_send_;

  
  bool initialized_;

  DISALLOW_COPY_AND_ASSIGN(FakeConnectionHandler);
};

}  

#endif  
