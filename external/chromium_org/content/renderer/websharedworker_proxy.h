// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_WEBSHAREDWORKER_PROXY_H_
#define CONTENT_RENDERER_WEBSHAREDWORKER_PROXY_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ipc/ipc_listener.h"
#include "third_party/WebKit/public/web/WebSharedWorkerConnector.h"
#include "url/gurl.h"

namespace content {

class ChildThread;

class WebSharedWorkerProxy : public blink::WebSharedWorkerConnector,
                             private IPC::Listener {
 public:
  
  WebSharedWorkerProxy(ChildThread* child_thread,
                       unsigned long long document_id,
                       bool exists,
                       int route_id,
                       int render_view_route_id);
  virtual ~WebSharedWorkerProxy();

  
  virtual bool isStarted();
  virtual void connect(blink::WebMessagePortChannel* channel,
                       ConnectListener* listener);

  virtual void startWorkerContext(
      const blink::WebURL& script_url,
      const blink::WebString& name,
      const blink::WebString& user_agent,
      const blink::WebString& source_code,
      const blink::WebString& content_security_policy,
      blink::WebContentSecurityPolicyType policy_type,
      long long script_resource_appcache_id);

 private:
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  bool IsStarted();

  
  void Disconnect();

  
  
  bool Send(IPC::Message*);

  
  bool HasQueuedMessages() { return !queued_messages_.empty(); }

  
  void SendQueuedMessages();

  void CreateWorkerContext(const GURL& script_url,
                           bool is_shared,
                           const base::string16& name,
                           const base::string16& user_agent,
                           const base::string16& source_code,
                           const base::string16& content_security_policy,
                           blink::WebContentSecurityPolicyType policy_type,
                           int pending_route_id,
                           int64 script_resource_appcache_id);
  void OnWorkerCreated();


  
  
  
  
  int route_id_;

  
  int render_view_route_id_;

  ChildThread* child_thread_;

  
  
  unsigned long long document_id_;

  
  std::vector<IPC::Message*> queued_messages_;

  
  
  
  int pending_route_id_;
  ConnectListener* connect_listener_;

  DISALLOW_COPY_AND_ASSIGN(WebSharedWorkerProxy);
};

}  

#endif  
