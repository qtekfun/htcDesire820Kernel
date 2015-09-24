// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_MESSAGING_MESSAGE_SERVICE_H_
#define CHROME_BROWSER_EXTENSIONS_API_MESSAGING_MESSAGE_SERVICE_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/extensions/api/messaging/message_property_provider.h"
#include "chrome/browser/extensions/api/messaging/native_message_process_host.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/common/extensions/api/messaging/message.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class GURL;
class Profile;

namespace base {
class DictionaryValue;
}

namespace content {
class RenderProcessHost;
class WebContents;
}

namespace extensions {
class Extension;
class ExtensionHost;
class LazyBackgroundTaskQueue;

class MessageService : public ProfileKeyedAPI,
                       public content::NotificationObserver,
                       public NativeMessageProcessHost::Client {
 public:
  
  
  
  struct MessageChannel;

  
  class MessagePort {
   public:
    virtual ~MessagePort() {}
    
    virtual void DispatchOnConnect(int dest_port_id,
                                   const std::string& channel_name,
                                   const base::DictionaryValue& source_tab,
                                   const std::string& source_extension_id,
                                   const std::string& target_extension_id,
                                   const GURL& source_url,
                                   const std::string& tls_channel_id) {}

    
    
    virtual void DispatchOnDisconnect(int source_port_id,
                                      const std::string& error_message) {}

    
    virtual void DispatchOnMessage(const Message& message,
                                   int target_port_id) = 0;

    
    
    virtual void IncrementLazyKeepaliveCount() {}
    virtual void DecrementLazyKeepaliveCount() {}

    
    virtual content::RenderProcessHost* GetRenderProcessHost();

   protected:
    MessagePort() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(MessagePort);
  };

  
  
  static void AllocatePortIdPair(int* port1, int* port2);

  explicit MessageService(Profile* profile);
  virtual ~MessageService();

  
  static ProfileKeyedAPIFactory<MessageService>* GetFactoryInstance();

  
  static MessageService* Get(Profile* profile);

  
  
  
  void OpenChannelToExtension(
      int source_process_id, int source_routing_id, int receiver_port_id,
      const std::string& source_extension_id,
      const std::string& target_extension_id,
      const GURL& source_url,
      const std::string& channel_name,
      bool include_tls_channel_id);

  
  
  
  void OpenChannelToTab(
      int source_process_id, int source_routing_id, int receiver_port_id,
      int tab_id, const std::string& extension_id,
      const std::string& channel_name);

  void OpenChannelToNativeApp(
      int source_process_id,
      int source_routing_id,
      int receiver_port_id,
      const std::string& source_extension_id,
      const std::string& native_app_name);

  
  
  virtual void CloseChannel(int port_id,
                            const std::string& error_message) OVERRIDE;

  
  
  void PostMessage(int port_id, const Message& message);

  
  virtual void PostMessageFromNativeProcess(
      int port_id,
      const std::string& message) OVERRIDE;

 private:
  friend class MockMessageService;
  friend class ProfileKeyedAPIFactory<MessageService>;
  struct OpenChannelParams;

  
  typedef std::map<int, MessageChannel*> MessageChannelMap;

  typedef std::pair<int, Message> PendingMessage;
  typedef std::vector<PendingMessage> PendingMessagesQueue;
  
  
  typedef std::map<int, PendingMessagesQueue> PendingTlsChannelIdMap;

  
  
  typedef std::string ExtensionID;
  typedef std::pair<Profile*, ExtensionID> PendingLazyBackgroundPageChannel;
  typedef std::map<int, PendingLazyBackgroundPageChannel>
      PendingLazyBackgroundPageChannelMap;

  
  bool OpenChannelImpl(scoped_ptr<OpenChannelParams> params);

  void CloseChannelImpl(MessageChannelMap::iterator channel_iter,
                        int port_id,
                        const std::string& error_message,
                        bool notify_other_port);

  
  
  void AddChannel(MessageChannel* channel, int receiver_port_id);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void OnProcessClosed(content::RenderProcessHost* process);

  void GotDomainBoundCert(scoped_ptr<OpenChannelParams> params,
                          const std::string& tls_channel_id);

  
  void EnqueuePendingMessage(int port_id, int channel_id,
                             const Message& message);

  
  void EnqueuePendingMessageForLazyBackgroundLoad(int port_id,
                                                  int channel_id,
                                                  const Message& message);

  
  void DispatchMessage(int port_id, MessageChannel* channel,
                       const Message& message);

  
  
  
  bool MaybeAddPendingLazyBackgroundPageOpenChannelTask(
      Profile* profile,
      const Extension* extension,
      OpenChannelParams* params);

  
  
  
  void PendingLazyBackgroundPageOpenChannel(
      scoped_ptr<OpenChannelParams> params,
      int source_process_id,
      extensions::ExtensionHost* host);
  void PendingLazyBackgroundPageCloseChannel(int port_id,
                                             const std::string& error_message,
                                             extensions::ExtensionHost* host) {
    if (host)
      CloseChannel(port_id, error_message);
  }
  void PendingLazyBackgroundPagePostMessage(int port_id,
                                            const Message& message,
                                            extensions::ExtensionHost* host) {
    if (host)
      PostMessage(port_id, message);
  }

  
  
  void DispatchOnDisconnect(content::RenderProcessHost* source,
                            int port_id,
                            const std::string& error_message);

  
  static const char* service_name() {
    return "MessageService";
  }
  static const bool kServiceRedirectedInIncognito = true;
  static const bool kServiceIsCreatedWithBrowserContext = false;
  static const bool kServiceIsNULLWhileTesting = true;

  content::NotificationRegistrar registrar_;
  MessageChannelMap channels_;
  PendingTlsChannelIdMap pending_tls_channel_id_channels_;
  PendingLazyBackgroundPageChannelMap pending_lazy_background_page_channels_;
  MessagePropertyProvider property_provider_;

  
  LazyBackgroundTaskQueue* lazy_background_task_queue_;

  base::WeakPtrFactory<MessageService> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(MessageService);
};

}  

#endif  
