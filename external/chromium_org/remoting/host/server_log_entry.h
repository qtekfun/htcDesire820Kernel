// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_SERVER_LOG_ENTRY_H_
#define REMOTING_HOST_SERVER_LOG_ENTRY_H_

#include <map>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "remoting/host/host_exit_codes.h"
#include "remoting/host/host_status_sender.h"
#include "remoting/protocol/transport.h"

namespace buzz {
class XmlElement;
}  

namespace remoting {

class ServerLogEntry {
 public:
  
  enum Mode {
    IT2ME,
    ME2ME
  };

  
  
  
  static scoped_ptr<buzz::XmlElement> MakeStanza();

  
  
  static scoped_ptr<ServerLogEntry> MakeForSessionStateChange(bool connection);

  
  static scoped_ptr<ServerLogEntry> MakeForHeartbeat();

  
  static scoped_ptr<ServerLogEntry> MakeForHostStatus(
      HostStatusSender::HostStatus host_status, HostExitCodes exit_code);

  ~ServerLogEntry();

  
  void AddHostFields();

  
  void AddModeField(Mode mode);

  
  void AddConnectionTypeField(protocol::TransportRoute::RouteType type);

  
  scoped_ptr<buzz::XmlElement> ToStanza() const;

 private:
  typedef std::map<std::string, std::string> ValuesMap;

  ServerLogEntry();
  void Set(const std::string& key, const std::string& value);

  static const char* GetValueSessionState(bool connected);
  static const char* GetValueMode(Mode mode);

  ValuesMap values_map_;
};

}  

#endif
