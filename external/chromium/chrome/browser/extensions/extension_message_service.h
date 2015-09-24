// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_MESSAGE_SERVICE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_MESSAGE_SERVICE_H_
#pragma once

#include <map>
#include <set>
#include <string>

#include "base/memory/ref_counted.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ipc/ipc_message.h"

class GURL;
class Profile;
class TabContents;


class ExtensionMessageService
    : public base::RefCounted<ExtensionMessageService>,
      public NotificationObserver {
 public:
  
  
  struct MessageChannel;
  struct MessagePort;

  
  static const char kDispatchOnConnect[];
  static const char kDispatchOnDisconnect[];
  static const char kDispatchOnMessage[];

  
  
  static void AllocatePortIdPair(int* port1, int* port2);

  explicit ExtensionMessageService(Profile* profile);

  
  void DestroyingProfile();

  
  
  
  void OpenChannelToExtension(
      int source_process_id, int source_routing_id, int receiver_port_id,
      const std::string& source_extension_id,
      const std::string& target_extension_id,
      const std::string& channel_name);

  
  
  
  void OpenChannelToTab(
      int source_process_id, int source_routing_id, int receiver_port_id,
      int tab_id, const std::string& extension_id,
      const std::string& channel_name);

  
  
  
  
  
  
  
  int OpenSpecialChannelToExtension(
      const std::string& extension_id, const std::string& channel_name,
      const std::string& tab_json, IPC::Message::Sender* source);

  
  
  
  
  
  
  
  int OpenSpecialChannelToTab(
      const std::string& extension_id, const std::string& channel_name,
      TabContents* target_tab_contents, IPC::Message::Sender* source);

  
  
  void CloseChannel(int port_id);

  
  void PostMessageFromRenderer(int port_id, const std::string& message);

 private:
  friend class base::RefCounted<ExtensionMessageService>;
  friend class MockExtensionMessageService;

  
  typedef std::map<int, MessageChannel*> MessageChannelMap;

  virtual ~ExtensionMessageService();

  
  bool OpenChannelImpl(
      IPC::Message::Sender* source,
      const std::string& tab_json,
      const MessagePort& receiver, int receiver_port_id,
      const std::string& source_extension_id,
      const std::string& target_extension_id,
      const std::string& channel_name);

  void CloseChannelImpl(MessageChannelMap::iterator channel_iter, int port_id,
                        bool notify_other_port);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void OnSenderClosed(IPC::Message::Sender* sender);

  Profile* profile_;

  NotificationRegistrar registrar_;

  MessageChannelMap channels_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionMessageService);
};

#endif  
