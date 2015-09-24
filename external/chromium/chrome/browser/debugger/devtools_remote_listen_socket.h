// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DEBUGGER_DEVTOOLS_REMOTE_LISTEN_SOCKET_H_
#define CHROME_BROWSER_DEBUGGER_DEVTOOLS_REMOTE_LISTEN_SOCKET_H_
#pragma once

#include <string>

#include "chrome/browser/debugger/devtools_remote_message.h"
#include "net/base/listen_socket.h"

class DevToolsRemoteListener;

class DevToolsRemoteListenSocket : public ListenSocket,
                                   public ListenSocket::ListenSocketDelegate {
 public:
  
  
  static DevToolsRemoteListenSocket* Listen(
      const std::string& ip,
      int port,
      DevToolsRemoteListener* message_listener);

 protected:
  virtual void Listen();
  virtual void Accept();
  virtual void Close();
  virtual void SendInternal(const char* bytes, int len);

 private:
  virtual ~DevToolsRemoteListenSocket();

  
  virtual void DidAccept(ListenSocket *server, ListenSocket *connection);
  virtual void DidRead(ListenSocket *connection, const char* data, int len);
  virtual void DidClose(ListenSocket *connection);

  
  enum State {
    INVALID = 0,  
    HANDSHAKE = 1,  
    HEADERS = 2,  
    PAYLOAD = 3  
  };

  DevToolsRemoteListenSocket(SOCKET s,
                             DevToolsRemoteListener *listener);
  void StartNextField();
  void HandleMessage();
  void DispatchField();
  const std::string& GetHeader(const std::string& header_name,
                               const std::string& default_value) const;

  State state_;
  DevToolsRemoteMessage::HeaderMap header_map_;
  std::string protocol_field_;
  std::string payload_;
  int32 remaining_payload_length_;
  DevToolsRemoteListener* message_listener_;
  bool cr_received_;

  DISALLOW_COPY_AND_ASSIGN(DevToolsRemoteListenSocket);
};

#endif  
